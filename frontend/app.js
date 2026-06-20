let currentResult = null;
let solveHistory = [];
const MAX_HISTORY = 20;
const TREND_POINTS = 60;
let trendData = [];
let autoRefreshTimer = null;

const commandLabels = {
  LED: "LED lighting",
  BUZZER: "Buzzer alert",
  FAN_PROXY: "Fan proxy",
  AC_PROXY: "AC proxy",
};

function byId(id) {
  return document.getElementById(id);
}

function num(value, fallback = 0) {
  const parsed = Number(value);
  return Number.isFinite(parsed) ? parsed : fallback;
}

function format(value, suffix, digits = 1) {
  if (value === undefined || value === null || Number.isNaN(Number(value))) {
    return "--";
  }
  return `${Number(value).toFixed(digits)}${suffix}`;
}

async function api(path, payload) {
  const options = payload === undefined
    ? {}
    : {
        method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify(payload),
      };
  const response = await fetch(path, options);
  const data = await response.json();
  if (!response.ok) {
    throw new Error(data.error || `HTTP ${response.status}`);
  }
  return data;
}

function snapshotFromForm() {
  const nodes = [...document.querySelectorAll(".node-card")].map((card) => ({
    id: card.dataset.nodeId,
    pir: Number(card.querySelector("[data-field='pir']").value),
    lux: num(card.querySelector("[data-field='lux']").value),
    temperature_c: num(card.querySelector("[data-field='temperature_c']").value),
    humidity_percent: num(card.querySelector("[data-field='humidity_percent']").value),
    bh_status: "ESP_OK",
    dht_status: "ESP_OK",
  }));

  return {
    price_yuan_per_kwh: num(byId("price").value, 1.2),
    power_limit_w: Math.round(num(byId("powerLimit").value, 1800)),
    target_temperature_c: num(byId("targetTemp").value, 26),
    nodes,
  };
}

function renderNodeForms(nodes) {
  const host = byId("nodeForms");
  host.innerHTML = "";

  nodes.forEach((node) => {
    const card = document.createElement("article");
    card.className = "node-card";
    card.dataset.nodeId = node.id;
    card.innerHTML = `
      <strong>ID ${node.id}</strong>
      <label>PIR
        <input data-field="pir" type="number" min="0" max="1" step="1" value="${node.pir}">
      </label>
      <label>Lux
        <input data-field="lux" type="number" step="0.1" value="${node.lux}">
      </label>
      <label>Temp C
        <input data-field="temperature_c" type="number" step="0.1" value="${node.temperature_c}">
      </label>
      <label>Humidity %
        <input data-field="humidity_percent" type="number" step="0.1" value="${node.humidity_percent}">
      </label>
    `;
    host.appendChild(card);
  });
}

function pushTrend(snapshot) {
  const avg = snapshot._derived_average || {};
  const now = Date.now();
  trendData.push({
    time: now,
    temperature_c: avg.temperature_c,
    lux: avg.lux,
    humidity_percent: avg.humidity_percent,
    occupied: avg.occupied ? 1 : 0,
  });
  if (trendData.length > TREND_POINTS) {
    trendData = trendData.slice(trendData.length - TREND_POINTS);
  }
}

function drawTrend() {
  const canvas = byId("trendChart");
  if (!canvas || trendData.length < 2) return;

  const ctx = canvas.getContext("2d");
  const rect = canvas.parentElement.getBoundingClientRect();
  const w = Math.max(rect.width - 36, 400);
  const h = 160;
  canvas.width = w;
  canvas.height = h;

  const pad = { top: 10, bottom: 24, left: 48, right: 16 };
  const pw = w - pad.left - pad.right;
  const ph = h - pad.top - pad.bottom;

  ctx.clearRect(0, 0, w, h);

  // Find ranges
  let tMin = Infinity, tMax = -Infinity;
  let hMin = Infinity, hMax = -Infinity;
  let lMax = -Infinity;
  trendData.forEach(d => {
    if (d.temperature_c < tMin) tMin = d.temperature_c;
    if (d.temperature_c > tMax) tMax = d.temperature_c;
    if (d.humidity_percent < hMin) hMin = d.humidity_percent;
    if (d.humidity_percent > hMax) hMax = d.humidity_percent;
    if (d.lux > lMax) lMax = d.lux;
  });
  tMin = Math.floor(tMin - 1);
  tMax = Math.ceil(tMax + 1);
  hMin = Math.floor(hMin - 5);
  hMax = Math.ceil(hMax + 5);
  if (tMin === tMax) tMax = tMin + 1;
  if (hMin === hMax) hMax = hMin + 1;

  const xScale = pw / (trendData.length - 1);
  const tempY = (v) => pad.top + ph - ((v - tMin) / (tMax - tMin)) * ph;
  const humY = (v) => pad.top + ph - ((v - hMin) / (hMax - hMin)) * ph;

  // Draw axes
  ctx.strokeStyle = "#d8dee9";
  ctx.lineWidth = 1;
  ctx.beginPath();
  ctx.moveTo(pad.left, pad.top);
  ctx.lineTo(pad.left, pad.top + ph);
  ctx.lineTo(pad.left + pw, pad.top + ph);
  ctx.stroke();

  // Y-axis labels for temp
  ctx.fillStyle = "#657184";
  ctx.font = "10px sans-serif";
  ctx.textAlign = "right";
  for (let t = Math.ceil(tMin); t <= Math.floor(tMax); t += Math.max(1, Math.round((tMax - tMin) / 4))) {
    const y = tempY(t);
    ctx.fillText(t + "C", pad.left - 4, y + 3);
  }

  // Temperature line
  ctx.strokeStyle = "#b42318";
  ctx.lineWidth = 2;
  ctx.beginPath();
  trendData.forEach((d, i) => {
    const x = pad.left + i * xScale;
    const y = tempY(d.temperature_c);
    i === 0 ? ctx.moveTo(x, y) : ctx.lineTo(x, y);
  });
  ctx.stroke();

  // Humidity line
  ctx.strokeStyle = "#0f766e";
  ctx.lineWidth = 2;
  ctx.beginPath();
  trendData.forEach((d, i) => {
    const x = pad.left + i * xScale;
    const y = humY(d.humidity_percent);
    i === 0 ? ctx.moveTo(x, y) : ctx.lineTo(x, y);
  });
  ctx.stroke();

  // Legend
  ctx.fillStyle = "#b42318";
  ctx.font = "11px sans-serif";
  ctx.textAlign = "left";
  ctx.fillRect(pad.left + pw - 130, pad.top + 4, 10, 10);
  ctx.fillText("Temp", pad.left + pw - 116, pad.top + 13);

  ctx.fillStyle = "#0f766e";
  ctx.fillRect(pad.left + pw - 76, pad.top + 4, 10, 10);
  ctx.fillText("Humidity", pad.left + pw - 62, pad.top + 13);
}

function renderSolveHistory() {
  const host = byId("solveHistory");
  host.innerHTML = "";

  solveHistory.slice(-MAX_HISTORY).reverse().forEach((entry) => {
    const item = document.createElement("div");
    item.className = "solve-history-item";

    const time = new Date(entry.timestamp);
    const timeStr = time.toLocaleTimeString();
    const commands = entry.solution.commands || {};

    let cmdsHtml = "";
    Object.entries(commands).forEach(([key, val]) => {
      cmdsHtml += `<span class="cmd-badge ${val ? 'on' : 'off'}">${key}</span>`;
    });

    item.innerHTML = `
      <div class="time">${timeStr} (${entry.solve_time_ms || "?"} ms)</div>
      <div class="objective">Obj: ${entry.solution.objective ?? "--"}</div>
      <div class="commands">${cmdsHtml}</div>
    `;
    host.appendChild(item);
  });
}

function render(result) {
  currentResult = result;
  const snapshot = result.snapshot || {};
  const avg = snapshot._derived_average || {};
  const solution = result.solution || {};

  byId("price").value = snapshot.price_yuan_per_kwh ?? 1.2;
  byId("powerLimit").value = snapshot.power_limit_w ?? 1800;
  byId("targetTemp").value = snapshot.target_temperature_c ?? 26;

  byId("avgTemp").textContent = format(avg.temperature_c, " C");
  byId("avgLux").textContent = format(avg.lux, " lx");
  byId("usedPower").textContent = `${solution.total_power_w ?? "--"} W`;
  byId("objective").textContent = solution.objective ?? "--";

  renderNodeForms(snapshot.nodes || []);

  const commands = byId("commands");
  commands.innerHTML = "";
  Object.entries(solution.commands || {}).forEach(([key, value]) => {
    const item = document.createElement("article");
    item.className = "command";
    item.innerHTML = `
      <strong>${commandLabels[key] || key}</strong>
      <span class="badge ${value ? "on" : "off"}">${value ? "ON" : "OFF"}</span>
    `;
    commands.appendChild(item);
  });

  byId("policyFrame").textContent = result.stm32_policy_frame || "--";

  const model = result.model || {};
  byId("modelInfo").innerHTML = `
    <div><strong>Decision variables</strong><code>${(model.decision_variables || []).join(", ")}</code></div>
    <div><strong>Objective</strong><code>${model.objective || "--"}</code></div>
    <div><strong>Constraints</strong><code>${(model.constraints || []).join("; ")}</code></div>
    <div><strong>Cost</strong><code>${solution.estimated_cost_yuan_per_hour || 0} yuan/hour</code></div>
  `;

  const reasons = byId("reasons");
  reasons.innerHTML = "";
  const list = solution.reasons && solution.reasons.length
    ? solution.reasons
    : ["No special trigger; optimizer used base device rewards and power limit."];
  list.forEach((reason) => {
    const li = document.createElement("li");
    li.textContent = reason;
    reasons.appendChild(li);
  });

  // Trend data
  pushTrend(snapshot);
  drawTrend();

  // Solve history
  if (result.solve_time_ms !== undefined) {
    const entry = {
      timestamp: Date.now(),
      solution: { ...solution },
      solve_time_ms: result.solve_time_ms,
    };
    solveHistory.push(entry);
    renderSolveHistory();
  }
}

async function loadState() {
  render(await api("/api/state"));
}

async function solveFromForm() {
  render(await api("/api/solve", {snapshot: snapshotFromForm()}));
}

async function applyFrame() {
  const frame = byId("rawFrame").value.trim();
  if (!frame) {
    return;
  }
  render(await api("/api/frame", {frame}));
}

async function resetDemo() {
  trendData = [];
  solveHistory = [];
  render(await api("/api/reset", {}));
}

function toggleAutoRefresh() {
  const cb = byId("autoRefresh");
  if (cb.checked) {
    autoRefreshTimer = setInterval(async () => {
      try {
        render(await api("/api/state"));
      } catch (e) {
        // Silently retry next interval
      }
    }, 5000);
  } else {
    if (autoRefreshTimer) {
      clearInterval(autoRefreshTimer);
      autoRefreshTimer = null;
    }
  }
}

byId("solveBtn").addEventListener("click", () => {
  solveFromForm().catch((error) => alert(error.message));
});

byId("applyFrameBtn").addEventListener("click", () => {
  applyFrame().catch((error) => alert(error.message));
});

byId("resetBtn").addEventListener("click", () => {
  resetDemo().catch((error) => alert(error.message));
});

byId("autoRefresh").addEventListener("change", toggleAutoRefresh);

loadState().catch((error) => {
  alert(`Local server is not running or API failed: ${error.message}`);
});
