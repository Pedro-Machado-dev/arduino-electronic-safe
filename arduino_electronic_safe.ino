//Incluindo as bibliotecas do Display LCD e do Servo Motor
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//Instanciando os objetos do Display LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Instanciando o objeto do Servo Motor
Servo motor;

//Definindo os pinos de cada componente
#define pot A0
#define botao1 2
#define botao2 3
#define vermelho 4
#define verde 5

//Declarando vetor para armazenar a senha correta: 0902
int senhaCorreta[4] = {0, 9, 0, 2};

//Declarando vetor que receberá os dígitos do usuário
int senhaDigitada[4] = { -1, -1, -1, -1};

//Declarando variável que vai armazenar a posição de cada dígito
//que o usuário vai inserir (Posições: 0, 1, 2 e 3 do vetor)
int posicao = 0;

void setup() {
  //Definindo as entradas e saídas do circuito
  pinMode(pot, INPUT);
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);

  pinMode(vermelho, OUTPUT);
  pinMode(verde, OUTPUT);

  //Definindo a porta em que o Servo Motor está ligado
  motor.attach(6);

  //Iniciando o motor na posição "travado" (180°)
  motor.write(180);

  //Inicializando o Display LCD
  lcd.init();

  //Ligando a luz de fundo do Display LCD
  lcd.backlight();

  //Colocando os títulos de cada linha pedido no enunciado
  lcd.setCursor(0, 0);
  lcd.print("Digite a senha:");
  lcd.setCursor(0, 1);
  lcd.print("Senha: _ _ _ _");
}

void loop() {
  //Criando variáveis para armazenar os estados dos botões
  int confirm = digitalRead(botao1);
  int reset = digitalRead(botao2);

  //Criando variável para armazenar o valor do potenciômetro
  int poten = analogRead(pot);

  //Criando variável para converter as leituras do potenciômetro
  //em números de 0 à 9 (para digitar a senha)
  int numero = map(poten, 0, 1023, 0, 9);

  //Mostra no Display LCD o número atual do potenciômetro
  //para que o usuário confirme ou não
  lcd.setCursor(0, 1);
  lcd.print("Senha: ");

  //Laço de repetição para passar pelas quatro posições da senha
  for (int i = 0; i < 4; i++)
  {
    //Se a posição já foi preenchida (ou seja, já confirmei o
    //dígito daquela posição)
    if (i < posicao and senhaDigitada[i] != -1)
    {
      //Mostra no Display LCD o dígito referente à posição do potenciômetro
      lcd.print(senhaDigitada[i]);
      //Espaço após o número para preencher no mesmo padrão
      //da mensagem inicial
      lcd.print(" ");
    }

    //Se for a posição atual que está sendo editada
    else if (i == posicao)
    {
      //Printa o número selecionado e confirmado pelo botão
      lcd.print(numero);
      //Espaço após o número para preencher no mesmo padrão
      //da mensagem inicial
      lcd.print(" ");
    }

    //Caso contrário (posição ainda não preenchida)
    else
    {
      //Mostra um traço, indicando posição vazia
      lcd.print("_ ");
    }
  }

  //Se o botão de confirmação for pressionado
  if (confirm == 1)
  {
    //O número que estava no Display fica confirmado na posição
    senhaDigitada[posicao] = numero;
    //Posição é incrementada no Display LCD e no vetor de senha
    posicao++;

    //Pequeno delay para que o botão não seja pressionado mais
    //de uma vez sem querer
    delay(250);

    //Se o vetor passou da última posição (que é 3)
    if (posicao == 4)
    {

      //Cria uma variável para verificar se a senha foi digitada
      //de forma correta ou não
      int correta = 1;

      //Laço de repetição para confirmar se todas as posições
      //da senha foram digitadas de forma correta
      for (int i = 0; i < 4; i++)
      {
        //Se algum dígito está incorreto = senha incorreta!
        if (senhaDigitada[i] != senhaCorreta[i])
        {
          //Zera a variável para indicar que a senha foi digitada
          //de forma incorreta
          correta = 0;
        }
      }

      //Apaga o que está no Display LCD para printar se a senha
      //está correta ou incorreta
      lcd.clear();

      //Se a senha estiver correta - Mensagem no Display LCD
      //LED verde acende e motor vai para a posição "destravado" (90°)
      if (correta == 1)
      {
        lcd.print("Senha correta!");
        digitalWrite(verde, HIGH);
        digitalWrite(vermelho, LOW);
        motor.write(90);
      }

      //Se a senha estiver incorreta - Mensagem no Display LCD
      //LED vermelho acende e motor continua na posição "travado" (180°)
      else
      {
        lcd.print("Senha incorreta!");
        digitalWrite(verde, LOW);
        digitalWrite(vermelho, HIGH);
        motor.write(180);
      }

      //Espera três segundos com a mensagem na tela e o LED em questão
      //aceso - Depois reseta o sistema
      delay(3000);

      //Laço de repetição para resetar o sistema (jogando todas as
      //posições para -1)
      for (int i = 0; i < 4; i++)
      {
        senhaDigitada[i] = -1;
      }

      //Posição volta para zero para digitar a senha novamente
      posicao = 0;

      //Apaga o que estava no Display e volta para mensagem inicial
      lcd.clear();
      lcd.print("Digite a senha:");
      lcd.setCursor(0, 1);
      lcd.print("Senha: _ _ _ _");

      //LEDs apagados e motor na posição "travado" (180°)
      digitalWrite(verde, LOW);
      digitalWrite(vermelho, LOW);
      motor.write(180);
    }
  }

  //Se o botão de reset for pressionado
  if (reset == 1)
  {
    //Posição volta para zero para digitar a senha novamente
    posicao = 0;

    //Laço de repetição para resetar o sistema (jogando todas as
    //posições para -1)
    for (int i = 0; i < 4; i++)
    {
      senhaDigitada[i] = -1;
    }

    //Mensagem no Display LCD - Sistema Resetado
    lcd.clear();
    lcd.print("Sistema Resetado");

    //LEDs verde e vermelho piscam três vezes para indicar que
    //o sistema está sendo resetado
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(verde, HIGH);
      digitalWrite(vermelho, HIGH);
      delay(500);
      digitalWrite(verde, LOW);
      digitalWrite(vermelho, LOW);
      delay(500);
    }

    //Motor volta fica na posição "travada" (180°)
    motor.write(180);

    //Apaga o que estava no Display e volta para mensagem inicial
    lcd.clear();
    lcd.print("Digite a senha:");
    lcd.setCursor(0, 1);
    lcd.print("Senha: _ _ _ _");
  }
}
