import { chromium } from "playwright";

export async function runAutomation() {

  const url =
    process.env.AUTOMATION_URL ||
    "http://localhost:3000/demo";

  console.log("Abrindo:", url);

  const browser = await chromium.launch({
    headless: false
  });

  const page = await browser.newPage();

  try {

    await page.goto(url, {
      waitUntil: "domcontentloaded"
    });

    // ==========================================
    // FLUXO DE TESTE
    // ==========================================

    // Procura o botão "Começar"
    await page
      .getByRole("button", { name: "Começar" })
      .click();

    // Procura o campo "Nome"
    await page
      .getByLabel("Nome")
      .fill("TESTE");

    // Procura "Continuar"
    await page
      .getByRole("button", { name: "Continuar" })
      .click();

    // Lê o texto da página
    const texto =
      await page
        .locator("#resultado")
        .innerText();

    console.log("Texto encontrado:", texto);

    // Coloca o texto no campo Código
    await page
      .getByLabel("Código")
      .fill(texto);

    // Clica em Finalizar
    await page
      .getByRole("button", { name: "Finalizar" })
      .click();

    console.log("Automação concluída.");

  } finally {

    // Mantém o navegador aberto por 5 segundos
    await page.waitForTimeout(5000);

    await browser.close();
  }
}
