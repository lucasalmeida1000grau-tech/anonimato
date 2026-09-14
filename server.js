import "dotenv/config";
import express from "express";
import { runAutomation } from "./automation.js";

const app = express();
const PORT = Number(process.env.PORT || 3000);

app.use(express.json());
app.use(express.static("public"));

let running = false;

app.post("/trigger", async (req, res) => {
  if (running) {
    return res.status(409).json({
      ok: false,
      message: "Automação já está executando."
    });
  }

  running = true;

  res.json({
    ok: true,
    message: "Automação iniciada."
  });

  try {
    await runAutomation();
  } catch (error) {
    console.error("Erro na automação:", error);
  } finally {
    running = false;
  }
});

app.get("/status", (req, res) => {
  res.json({
    running
  });
});

app.listen(PORT, "0.0.0.0", () => {
  console.log(`Servidor: http://localhost:${PORT}`);
  console.log(`Gatilho ESP32: POST http://SEU_IP:${PORT}/trigger`);
});
