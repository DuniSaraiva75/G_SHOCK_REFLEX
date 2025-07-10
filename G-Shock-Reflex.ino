
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 3, 4, 5, 6, 7);

const int botaoPin = A5;
const int buzzerPin = 9;

void (*funcao_Reset)() = 0; // Ponteiro para reset

unsigned long tempoInicio = 0;
unsigned long tempoFim = 0;

unsigned long somaJ1 = 0;
unsigned long somaJ2 = 0;

int rodadaAtual = 1;
const int totalRodadas = 10;

int jogador = 1;
bool aguardandoInicio = true;
bool aguardandoFim = false;

void setup() {
  lcd.begin(16, 2);
  pinMode(botaoPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  lcd.print("G-Shock Reflex!");
  delay(2000);
  lcd.clear();
}

void loop() {
  if (rodadaAtual > totalRodadas) {
    mostrarResultadoFinal();
  }

  if (aguardandoInicio) {
    lcd.setCursor(0, 0);
    lcd.print("Rodada ");
    lcd.print(rodadaAtual);
    lcd.setCursor(0, 1);
    lcd.print("Jogador ");
    lcd.print(jogador);

    if (digitalRead(botaoPin) == LOW) {
      delay(200);
      tone(buzzerPin, 1000, 100);
      tempoInicio = millis();
      aguardandoInicio = false;
      aguardandoFim = true;
      lcd.clear();
      lcd.print("Cronometro ON!");
    }
  }

  if (aguardandoFim) {
    if (digitalRead(botaoPin) == LOW) {
      delay(200);
      tone(buzzerPin, 1000, 100);
      tempoFim = millis();
      unsigned long tempoReacao = tempoFim - tempoInicio;

      lcd.clear();
      lcd.print("Tempo:");
      lcd.setCursor(0, 1);
      lcd.print(tempoReacao);
      lcd.print(" ms");
      delay(2000);

      if (jogador == 1) {
        somaJ1 += tempoReacao;
        jogador = 2;
      } else {
        somaJ2 += tempoReacao;
        jogador = 1;
        rodadaAtual++;
      }

      aguardandoInicio = true;
      aguardandoFim = false;
      lcd.clear();
    }
  }
}

void mostrarResultadoFinal() {
  lcd.clear();
  lcd.print("Calculando...");
  delay(1000);

  unsigned long mediaJ1 = somaJ1 / totalRodadas;
  unsigned long mediaJ2 = somaJ2 / totalRodadas;

  lcd.clear();
  lcd.print("J1 Media:");
  lcd.setCursor(0, 1);
  lcd.print(mediaJ1);
  lcd.print(" ms");
  delay(3000);

  lcd.clear();
  lcd.print("J2 Media:");
  lcd.setCursor(0, 1);
  lcd.print(mediaJ2);
  lcd.print(" ms");
  delay(3000);

  lcd.clear();
  if (mediaJ1 < mediaJ2) {
    lcd.print("J1 VENCEU!");
    tocarMusicaVitoria();
  } else if (mediaJ2 < mediaJ1) {
    lcd.print("J2 VENCEU!");
    tocarMusicaVitoria();
  } else {
    lcd.print("EMPATE!");
    tone(buzzerPin, 500, 500);
  }

  delay(5000);
  lcd.clear();
  lcd.print("Press p/reiniciar");

  while (digitalRead(botaoPin) == HIGH) { }
  tone(buzzerPin, 1000, 100);
  delay(200);

  funcao_Reset();
}

void tocarMusicaVitoria() {
  int notas[] = { 1000, 1200, 1400, 1600, 1800, 2000 };
  int duracao = 100;
  for (int i = 0; i < 6; i++) {
    tone(buzzerPin, notas[i], duracao);
    delay(duracao + 50);
  }
}
