# NodeControl Framework

Framework C++ orientado a eventos para controle, comunicação e gerenciamento de dispositivos embarcados baseados em microcontroladores.

O projeto tem como objetivo criar uma arquitetura modular para dispositivos compostos por sensores e atuadores, chamados de **Nodes**, permitindo que um dispositivo se apresente a um Host, informe suas capacidades, mantenha seu estado operacional, monitore a comunicação e processe comandos de forma independente do meio físico utilizado.

Atualmente o desenvolvimento está sendo realizado sobre **Arduino**, utilizando comunicação Serial, mas a arquitetura procura manter protocolo, transporte, controle de estados e Nodes desacoplados para permitir futuramente outros meios de comunicação, como I²C e SPI.

> **Status:** Em desenvolvimento.
> O núcleo de comunicação, eventos, máquina de estados, heartbeat e configuração já está funcional. A camada `NodeController` e a interface `INode` estão em desenvolvimento.

---

# 1. Objetivos

O framework busca separar claramente as responsabilidades de um dispositivo embarcado.

Em vez de concentrar comunicação, timeouts, sensores, comandos e mudanças de estado dentro do `loop()`, o sistema é dividido em componentes especializados que se comunicam principalmente através de **eventos**.

A arquitetura pretende fornecer:

* máquina de estados do dispositivo;
* comunicação desacoplada do transporte físico;
* protocolo textual;
* arquitetura orientada a eventos;
* monitoramento de heartbeat;
* handshake de inicialização e configuração;
* descoberta das capacidades do dispositivo;
* gerenciamento de sensores e atuadores;
* comandos GET e SET sobre propriedades;
* publicação periódica dos estados dos Nodes;
* suporte a triggers gerados pelos Nodes;
* mensagens de erro e respostas;
* integração futura com um Host e MQTT.

---

# 2. Visão geral da arquitetura

A arquitetura atual pode ser representada como:

```text
                       ┌──────────────────┐
                       │       Host       │
                       └────────┬─────────┘
                                │
                         Serial / I²C / SPI
                                │
                                ▼
                       ┌──────────────────┐
                       │    Transport     │
                       └────────┬─────────┘
                                │
                                ▼
                       ┌──────────────────┐
                       │ ProtocolHandler  │
                       └────────┬─────────┘
                                │
                              Event
                                │
                                ▼
                    ┌──────────────────────┐
                    │     Orchestrator     │
                    └──────────┬───────────┘
                               │
             ┌─────────────────┼─────────────────┐
             │                 │                 │
             ▼                 ▼                 ▼
      ┌─────────────┐   ┌──────────────┐  ┌──────────────┐
      │StateMachine │   │ConfigMonitor │  │HeartBeat     │
      │             │   │              │  │Monitor       │
      └─────────────┘   └──────────────┘  └──────────────┘
                               │
                               │
                         ┌─────┴─────┐
                         ▼           ▼
                  ┌─────────────────────┐
                  │   NodeController    │
                  └──────────┬──────────┘
                             │
                    ┌────────┼────────┐
                    ▼        ▼        ▼
                   LED    Temperature Button
                    │        │        │
                    └────── INode ────┘
```

O princípio principal é:

> Cada componente deve conhecer apenas aquilo que pertence à sua responsabilidade.

---

# 3. Princípios arquiteturais

## IS-A

Herança é utilizada quando existe uma relação real de especialização.

Exemplo:

```text
LedNode IS-A INode
TemperatureNode IS-A INode
ButtonNode IS-A INode
```

Isso permite que o `NodeController` trabalhe com diferentes tipos de Nodes através de uma interface comum.

## HAS-A

Composição é utilizada para construir componentes maiores a partir de componentes especializados.

Exemplo conceitual:

```text
Orchestrator
    HAS-A StateMachine
    HAS-A ProtocolHandler
    HAS-A HeartBeatMonitor
    HAS-A ConfigMonitor
    HAS-A NodeController
```

O `Orchestrator` não é nenhum desses componentes. Ele apenas os possui e coordena.

---

# 4. Orchestrator

O `Orchestrator` é o componente central de coordenação.

Sua responsabilidade não é implementar a lógica interna dos módulos, mas:

* atualizar os componentes;
* capturar eventos;
* encaminhar eventos;
* coordenar ações entre subsistemas;
* aplicar regras globais do dispositivo.

Fluxo típico:

```text
module.update()
      │
      ▼
hasEvent()?
      │
      ▼
getEvent()
      │
      ▼
Orchestrator::handleEvent()
      │
      ├── StateMachine
      ├── ConfigMonitor
      ├── HeartBeatMonitor
      ├── ProtocolHandler
      └── NodeController
```

Essa abordagem evita dependências diretas desnecessárias entre os componentes.

---

# 5. Sistema de eventos

Os módulos se comunicam através de eventos.

Conceitualmente:

```cpp
struct Event
{
    EventType type;
    // payload quando necessário
};
```

Entre os eventos utilizados atualmente estão eventos relacionados a:

```text
UP_ACK
CFG_ACK

UP_TIMEOUT
CONFIG_TIMEOUT

HB_REQUEST
HB_ACK
HB_TIMEOUT

RUN
IDLE
REBOOT

GET_STATE

CMD_RCV
CMD_ACK
CMD_ERROR
```

O conjunto exato continuará evoluindo conforme a camada de Nodes for finalizada.

---

# 6. EventEmitter

Componentes capazes de produzir eventos utilizam um `EventEmitter`.

O padrão básico é:

```text
Component
    │
    ├── update()
    ├── handleEvent()
    │
    └── EventEmitter
            │
            ├── emitEvent()
            ├── hasEvent()
            └── getEvent()
```

Isso padroniza a maneira como os componentes notificam o `Orchestrator`.

A implementação atual trabalha com um evento pendente por emissor. Uma fila de eventos poderá ser considerada futuramente caso a necessidade seja demonstrada.

---

# 7. StateMachine

A `StateMachine` representa o estado operacional do dispositivo.

Estados atualmente previstos:

```text
STATE_UP
STATE_CFG
STATE_RUN
STATE_IDLE
STATE_DISABLED
```

A máquina é essencialmente **reativa**.

Ela não deve controlar timers, comunicação ou sensores diretamente.

Seu comportamento básico é:

```text
estado atual + evento
        │
        ▼
   decisão
        │
        ▼
novo estado
```

---

# 8. Fluxo de inicialização

O handshake básico é:

```text
NODE                                      HOST

STATE_UP
   │
   │ UP
   ├─────────────────────────────────────►
   │
   │◄────────────────────────────────────┤
   │               UP_ACK
   ▼
STATE_CFG
   │
   │ configuração/capacidades
   ├─────────────────────────────────────►
   │
   │◄────────────────────────────────────┤
   │               CFG_ACK
   ▼
STATE_RUN
```

O dispositivo somente entra em operação normal depois que o Host reconhece sua configuração.

---

# 9. STATE_UP

`STATE_UP` representa um dispositivo disponível para iniciar o handshake.

Enquanto permanece nesse estado, o dispositivo publica periodicamente sua condição para permitir que um Host o descubra.

```text
STATE_UP
    │
    ▼
UP_REQUEST
    │
    ▼
"UP"
    │
    ▼
Host
```

Após:

```text
UP_ACK
```

a máquina entra em:

```text
STATE_CFG
```

---

# 10. STATE_CFG

`STATE_CFG` representa a fase de negociação das capacidades do dispositivo.

O Node informa ao Host:

* quais Nodes possui;
* quais propriedades existem;
* seus tipos;
* quais propriedades podem ser alteradas;
* outras informações necessárias para sua utilização.

O dispositivo aguarda:

```text
CFG_ACK
```

Se recebido corretamente:

```text
STATE_CFG
    │
 CFG_ACK
    │
    ▼
STATE_RUN
```

Se não houver resposta dentro do tempo esperado:

```text
STATE_CFG
    │
CONFIG_TIMEOUT
    │
    ▼
STATE_UP
```

Isso impede que o dispositivo permaneça indefinidamente em uma negociação incompleta.

---

# 11. ConfigMonitor

A temporização do handshake não pertence à `StateMachine`.

Ela é responsabilidade do `ConfigMonitor`.

O monitor acompanha as fases relacionadas a:

```text
UP
CFG
```

e produz eventos quando necessário.

Exemplo:

```text
STATE_CFG
    │
aguarda CFG_ACK
    │
    ├── CFG_ACK ──────────► STATE_RUN
    │
    └── timeout ──────────► EVENT_CONFIG_TIMEOUT
                                  │
                                  ▼
                              STATE_UP
```

Essa separação mantém a máquina de estados livre de dependências temporais como `millis()`.

---

# 12. HeartBeatMonitor

O `HeartBeatMonitor` verifica a disponibilidade da comunicação.

Conceitualmente:

```text
tempo
 │
 ▼
EVENT_HB_REQUEST
 │
 ▼
Orchestrator
 │
 ▼
ProtocolHandler
 │
 ▼
HB
```

O Host responde:

```text
HB_ACK
```

Caso a resposta não seja recebida:

```text
HB
 │
 │ sem ACK
 ▼
EVENT_HB_TIMEOUT
 │
 ▼
Orchestrator
 │
 ▼
StateMachine
 │
 ▼
STATE_UP
```

Heartbeat e estado possuem significados diferentes:

```text
Heartbeat → liveness
"o outro lado está acessível?"

State → readiness
"em qual condição operacional estou?"
```

Um dispositivo pode estar vivo e ainda não estar pronto para operar.

---

# 13. Transport

O transporte é responsável somente pela movimentação de bytes.

Ele não deve conhecer:

* estados;
* comandos;
* Nodes;
* heartbeat;
* JSON;
* significado das mensagens.

Atualmente é utilizada comunicação Serial.

A arquitetura pretende permitir futuramente:

```text
SerialTransport
I2CTransport
SPITransport
...
```

sem alterar a lógica do protocolo.

---

# 14. ProtocolHandler

O `ProtocolHandler` transforma o fluxo de bytes em mensagens do protocolo.

Fluxo de recepção:

```text
Transport
    │
   byte
    ▼
receive()
    │
    ▼
buffer
    │
    ▼
framing
    │
    ▼
processMessage()
    │
    ▼
ProtocolMessage
    │
    ▼
Event
```

O terminador atual das mensagens textuais é:

```text
\n
```

Enquanto:

```text
\0
```

é utilizado internamente para finalizar strings C.

---

# 15. ProtocolMessage

Os tipos de mensagem são representados internamente através de um `enum class`.

Exemplo conceitual:

```cpp
enum class ProtocolMessage
{
    MESSAGE_UNKNOWN,

    MESSAGE_UP,
    MESSAGE_CFG,

    MESSAGE_ACK_UP,
    MESSAGE_ACK_CFG,

    MESSAGE_HB,
    MESSAGE_ACK_HB,

    MESSAGE_RUN,
    MESSAGE_IDLE,

    MESSAGE_REBOOT,
    MESSAGE_GET_STATE,

    MESSAGE_CMD
};
```

Os valores numéricos do enum **não são transmitidos**.

O protocolo no fio continua textual:

```text
UP
UP_ACK
CFG_ACK
HB
HB_ACK
RUN
IDLE
REBOOT
GET_STATE
```

Isso permite que Host e dispositivo sejam implementados em linguagens diferentes sem depender da representação binária de um enum C++.

---

# 16. Representação interna x representação no fio

O sistema separa três conceitos:

```text
WIRE                    PROTOCOL                   APPLICATION

"UP_ACK\n"
    │
    ▼
ProtocolMessage::...
    │
    ▼
EventType::EVENT_UP_ACK
```

Isso evita espalhar comparações de strings por toda a aplicação.

---

# 17. Configuração dos Nodes

O dispositivo será autodescritivo.

Uma configuração pode ser armazenada como JSON estático no firmware e enviada durante `STATE_CFG` ou quando explicitamente solicitada.

Exemplo:

```json
{
  "nodes": {
    "led": {
      "name": "Light",
      "properties": {
        "status": {
          "name": "status",
          "datatype": "boolean",
          "settable": true
        }
      }
    }
  }
}
```

Essa configuração descreve:

```text
Node: led
 └── Property: status
      ├── datatype: boolean
      └── settable: true
```

Ela representa **o que o dispositivo é capaz de fazer**, não necessariamente seu estado atual.

---

# 18. Configuração x dados

Existem duas informações distintas.

## Configuração

Descreve estrutura e capacidades:

```text
"Quem sou e o que ofereço?"
```

Exemplo:

```json
{
  "nodes": {
    "led": {
      "properties": {
        "status": {
          "datatype": "boolean",
          "settable": true
        }
      }
    }
  }
}
```

## Dados

Representam o estado atual:

```text
"Como estou agora?"
```

Exemplo:

```json
{
  "nodes": {
    "led": {
      "status": true
    },
    "temperature": {
      "value": 24.7
    },
    "humidity": {
      "value": 63.2
    }
  }
}
```

O objetivo é permitir publicação agregada periódica durante `STATE_RUN`.

---

# 19. Comandos

O formato em desenvolvimento é:

```text
CMD:<path>[:payload]
```

Exemplo SET:

```text
CMD:/led/status:true
```

Representação:

```text
path    = /led/status
payload = true
```

Exemplo GET:

```text
CMD:/led/status
```

Representação:

```text
path       = /led/status
hasPayload = false
```

Assim, a presença do payload diferencia inicialmente uma operação de escrita de uma consulta.

---

# 20. Command

O comando pode ser representado internamente por uma estrutura semelhante a:

```cpp
struct Command
{
    static constexpr size_t PATH_SIZE = 48;
    static constexpr size_t PAYLOAD_SIZE = 64;

    char path[PATH_SIZE]{};
    char payload[PAYLOAD_SIZE]{};

    bool hasPayload = false;
};
```

Exemplo:

```text
CMD:/led/status:true
```

torna-se:

```text
Command
 ├── path       = "/led/status"
 ├── payload    = "true"
 └── hasPayload = true
```

O `ProtocolHandler` não interpreta o datatype do valor.

Por exemplo:

```text
true
120
24.7
Hello
```

continuam sendo payloads textuais.

A interpretação pertence ao Node responsável pela propriedade.

---

# 21. Regras de execução de CMD

Comandos destinados aos Nodes somente podem ser executados quando o dispositivo estiver operacional.

Estados permitidos:

```text
STATE_RUN
STATE_IDLE
```

Estados como:

```text
STATE_UP
STATE_CFG
STATE_DISABLED
```

não devem executar comandos de Nodes.

Essa política é aplicada antes que o comando chegue ao `NodeController`.

Fluxo:

```text
EVENT_CMD_RCV
      │
      ▼
Orchestrator
      │
      ▼
verifica StateMachine
      │
 ┌────┴────┐
 │         │
RUN/IDLE   outros
 │         │
 ▼         ▼
Node       CMD_ERROR
Controller INVALID_STATE
```

---

# 22. NodeController

O `NodeController` é responsável por gerenciar os Nodes disponíveis no dispositivo.

Ele deverá:

* inicializar Nodes;
* atualizá-los;
* resetá-los;
* receber comandos;
* localizar os Nodes interessados;
* encaminhar comandos;
* coletar resultados;
* produzir eventos;
* coletar dados e triggers dos Nodes.

Conceitualmente:

```text
                 NodeController
                       │
              array de INode*
                       │
          ┌────────────┼────────────┐
          ▼            ▼            ▼
       LedNode    Temperature    ButtonNode
```

---

# 23. INode

Os diferentes sensores e atuadores devem compartilhar uma interface comum.

Versão inicial:

```cpp
class INode
{
public:
    virtual ~INode() = default;

    virtual void begin() = 0;
    virtual void update() = 0;
    virtual void reset() = 0;

    virtual const char* getId() const = 0;

    virtual bool accepts(const Command& command) const = 0;

    virtual bool handleCommand(const Command& command) = 0;
};
```

Exemplos:

```text
LedNode         IS-A INode
ButtonNode      IS-A INode
ThermometerNode IS-A INode
GasNode         IS-A INode
```

---

# 24. Registro de Nodes

O `NodeController` poderá armazenar referências para diferentes implementações através de ponteiros para `INode`.

Exemplo conceitual:

```cpp
INode* m_nodes[MAX_NODES];
```

Registro:

```cpp
nodeController.addNode(led);
nodeController.addNode(thermometer);
nodeController.addNode(button);
```

O controlador pode então iterar:

```cpp
for (size_t i = 0; i < m_nodeCount; ++i)
{
    if (m_nodes[i]->accepts(command))
    {
        m_nodes[i]->handleCommand(command);
    }
}
```

Não é obrigatório interromper após o primeiro Node.

Isso permitirá futuramente comandos que possam ser aceitos por múltiplos Nodes.

---

# 25. LedNode

O primeiro Node utilizado para validar a arquitetura é um LED.

Exemplo:

```text
/led/status
```

Operações previstas:

```text
CMD:/led/status:true
CMD:/led/status:false
CMD:/led/status
```

As duas primeiras alteram a propriedade.

A última consulta seu valor atual.

Exemplo de estrutura:

```cpp
class LedNode : public INode
{
public:
    explicit LedNode(uint8_t pin);

    void begin() override;
    void update() override;
    void reset() override;

    const char* getId() const override;

    bool accepts(const Command& command) const override;
    bool handleCommand(const Command& command) override;

private:
    uint8_t m_pin;
    bool m_status = false;

    void setStatus(bool status);
};
```

---

# 26. Respostas de comandos

A simples resposta `bool` de `handleCommand()` deverá evoluir para uma estrutura capaz de representar diferentes resultados.

Conceitualmente:

```text
COMMAND
   │
   ▼
Node
   │
   ├── SUCCESS
   │
   ├── RESPONSE
   │
   └── ERROR
```

Exemplo SET:

```text
CMD:/led/status:true
        │
        ▼
      LedNode
        │
        ▼
     SUCCESS
        │
        ▼
     CMD_ACK
```

Exemplo GET:

```text
CMD:/led/status
        │
        ▼
      LedNode
        │
        ▼
     RESPONSE
        │
        ▼
CMD_RESPONSE:/led/status:true
```

Exemplo inválido:

```text
CMD:/led/status:banana
        │
        ▼
      LedNode
        │
        ▼
 INVALID_VALUE
        │
        ▼
CMD_ERROR:INVALID_VALUE
```

---

# 27. Erros

Erros devem possuir identificadores estáveis para comunicação entre máquinas.

Exemplos planejados:

```text
CMD_ERROR:NODE_NOT_FOUND
CMD_ERROR:PROPERTY_NOT_FOUND
CMD_ERROR:PROPERTY_NOT_SETTABLE
CMD_ERROR:INVALID_VALUE
CMD_ERROR:INVALID_STATE
```

Erros gerais poderão utilizar:

```text
ERROR:INVALID_MESSAGE
ERROR:BUFFER_OVERFLOW
ERROR:PROTOCOL_ERROR
```

Mensagens humanas poderão futuramente ser adicionadas como informação complementar, mas o código de erro deve permanecer estável.

---

# 28. Triggers

Nem toda comunicação nasce de um comando do Host.

Um Node pode detectar espontaneamente um acontecimento.

Exemplo:

```text
ButtonNode
    │
 botão pressionado
    │
    ▼
EVENT_NODE_TRIGGER
    │
    ▼
NodeController
    │
    ▼
Orchestrator
    │
    ▼
ProtocolHandler
    │
    ▼
Host
```

Um trigger não deve ser confundido internamente com `CMD`.

Semanticamente:

```text
CMD
Host → Device
"faça isto"

DATA
Device → Host
"este é meu valor"

TRIGGER
Device → Host
"isto aconteceu"
```

O formato final dessas mensagens ainda está em definição.

---

# 29. Publicação periódica

Durante:

```text
STATE_RUN
```

o `NodeController` poderá coletar periodicamente os valores dos Nodes e construir um payload agregado.

Exemplo:

```json
{
  "nodes": {
    "led": {
      "status": true
    },
    "temperature": {
      "value": 24.7
    },
    "humidity": {
      "value": 63.2
    },
    "gas": {
      "value": 418
    }
  }
}
```

Isso evita necessariamente transmitir uma mensagem completa para cada sensor em cada ciclo.

---

# 30. Integração futura com MQTT

O dispositivo embarcado não deverá conhecer MQTT.

A arquitetura planejada é:

```text
DEVICE
   │
Serial / I²C / SPI
   │
   ▼
HOST
   │
 MQTT
   │
   ▼
BROKER
```

O Host atuará como gateway entre o protocolo interno e MQTT.

Exemplo MQTT:

```text
Topic:
FCA/UL51/ESP3243/led/status

Payload:
true
```

O Host poderá retirar o contexto que pertence ao MQTT:

```text
FCA/UL51/ESP3243
```

e transformar a operação para o protocolo do dispositivo:

```text
CMD:/led/status:true
```

O dispositivo conhece apenas:

```text
/led/status
```

e não precisa conhecer:

```text
FCA
UL51
ESP3243
MQTT
Broker
```

---

# 31. Fluxo completo de comando

Objetivo arquitetural:

```text
MQTT

FCA/UL51/ESP3243/led/status
payload = true

             │
             ▼
            Host
             │
             ▼
CMD:/led/status:true
             │
             ▼
          Transport
             │
             ▼
      ProtocolHandler
             │
             ▼
       EVENT_CMD_RCV
             │
             ▼
        Orchestrator
             │
             ▼
       NodeController
             │
             ▼
          LedNode
             │
             ▼
          LED ON
             │
             ▼
        CommandResult
             │
             ▼
        NodeController
             │
             ▼
        Orchestrator
             │
             ▼
      ProtocolHandler
             │
             ▼
           Host
```

---

# 32. Fluxo espontâneo de dados

No sentido contrário:

```text
Sensors / Actuators
         │
         ▼
       Nodes
         │
         ▼
  NodeController
         │
         ├── DATA
         └── TRIGGER
         │
         ▼
   Orchestrator
         │
         ▼
 ProtocolHandler
         │
         ▼
       Host
         │
         ▼
       MQTT
```

---

# 33. REBOOT

`REBOOT` é tratado como um comando global.

Seu significado não se limita à máquina de estados.

Conceitualmente:

```text
REBOOT
   │
   ├── StateMachine      → STATE_UP
   ├── ConfigMonitor     → reinicia handshake
   ├── HeartBeatMonitor  → reinicia monitoramento
   └── NodeController    → reset dos Nodes
```

Isso permitirá, por exemplo, que um atuador retorne a uma posição conhecida durante a reinicialização lógica do sistema.

---

# 34. Validação realizada

O núcleo já foi testado através da comunicação Serial.

Foram validados:

* envio e recebimento de bytes;
* framing das mensagens;
* interpretação pelo `ProtocolHandler`;
* transformação de mensagens em eventos;
* captura dos eventos pelo `Orchestrator`;
* encaminhamento dos eventos;
* transições da `StateMachine`;
* `UP → CFG`;
* `CFG → RUN`;
* `RUN ↔ IDLE`;
* heartbeat;
* `HB_ACK`;
* `HB_TIMEOUT`;
* retorno para `STATE_UP`;
* timeout de configuração;
* reboot;
* consulta de estado;
* processamento inicial de comandos destinados aos Nodes.

Isso valida o fluxo fundamental:

```text
mensagem
   ↓
Transport
   ↓
ProtocolHandler
   ↓
Event
   ↓
Orchestrator
   ↓
Component
   ↓
ação
```

---

# 35. Estrutura conceitual do projeto

A organização poderá evoluir, mas atualmente o projeto segue conceitualmente algo próximo de:

```text
src/
├── core/
│   ├── Events
│   ├── EventEmitter
│   ├── StateMachine
│   └── Orchestrator
│
├── protocol/
│   ├── ProtocolHandler
│   ├── ProtocolMessages
│   ├── ITransport
│   └── SerialTransport
│
├── monitors/
│   ├── ConfigMonitor
│   └── HeartBeatMonitor
│
├── nodes/
│   ├── INode
│   ├── NodeController
│   ├── LedNode
│   └── ...
│
└── main.cpp
```

A estrutura definitiva será consolidada conforme a camada de Nodes for finalizada.

---

# 36. Filosofia do projeto

Algumas decisões orientam o desenvolvimento:

### Separação de responsabilidades

Cada classe deve possuir uma responsabilidade clara.

### Baixo acoplamento

Sensores não devem conhecer protocolo.

O protocolo não deve conhecer sensores.

A máquina de estados não deve conhecer Serial.

O transporte não deve conhecer mensagens.

### Composição antes de herança

`HAS-A` é utilizado para construir componentes.

Herança é reservada para relações `IS-A` reais, como diferentes implementações de `INode`.

### Event-driven

Módulos notificam acontecimentos através de eventos em vez de controlar diretamente outros módulos.

### Non-blocking

Timers utilizam o modelo baseado em `millis()` e `update()`, evitando `delay()` e esperas bloqueantes.

### Sem alocação dinâmica desnecessária

A arquitetura prioriza estruturas de tamanho conhecido, buffers fixos e comportamento previsível, adequado a microcontroladores.

### Protocolo independente do transporte

O mesmo protocolo deverá poder operar sobre diferentes transportes.

### Dispositivo independente de MQTT

MQTT pertence ao Host, não ao firmware dos Nodes.

---

# 37. Roadmap

## NodeControl

Em desenvolvimento:

* [x] StateMachine
* [x] Event system
* [x] EventEmitter
* [x] Orchestrator
* [x] Serial transport
* [x] ProtocolHandler
* [x] ProtocolMessage
* [x] HeartBeatMonitor
* [x] ConfigMonitor
* [x] UP/CFG handshake
* [x] heartbeat timeout
* [x] configuration timeout
* [x] RUN/IDLE
* [x] REBOOT
* [x] GET_STATE
* [x] parsing inicial de CMD
* [ ] consolidar `Command`
* [ ] implementar `CommandResult`
* [ ] consolidar `INode`
* [ ] finalizar `LedNode`
* [ ] finalizar `NodeController`
* [ ] CMD_ACK
* [ ] CMD_RESPONSE
* [ ] CMD_ERROR
* [ ] sistema de DATA
* [ ] sistema de TRIGGER
* [ ] JSON de configuração
* [ ] JSON agregado de estados
* [ ] testes completos da camada de Nodes

## HostDevice

Próxima etapa:

* [ ] arquitetura do Host
* [ ] gerenciamento de dispositivos
* [ ] implementação do protocolo do lado Host
* [ ] descoberta de dispositivos
* [ ] armazenamento das configurações
* [ ] roteamento de comandos
* [ ] MQTT
* [ ] tradução MQTT ↔ protocolo interno
* [ ] gerenciamento de ACKs
* [ ] tratamento de timeouts
* [ ] recuperação de sincronização

## Transportes futuros

* [x] Serial
* [ ] I²C
* [ ] SPI
* [ ] outros transportes

---

# 38. Estado atual

O projeto encontra-se no final da primeira grande etapa:

```text
NODE CONTROL FRAMEWORK
```

O núcleo operacional está funcionando e foi validado através de testes reais pela Serial.

O desenvolvimento atual está concentrado na generalização da camada:

```text
NodeController
      │
      ▼
    INode
      │
 ┌────┼─────────────┐
 ▼    ▼             ▼
LED  Sensors     Actuators
```

Após a conclusão dessa camada, o próximo objetivo será desenvolver:

```text
HostDevice
```

que atuará como gateway entre os dispositivos embarcados e sistemas externos, inicialmente através de MQTT.

---

# 39. Visão de longo prazo

O objetivo final é permitir construir dispositivos embarcados autodescritivos e modulares.

Um dispositivo poderá iniciar, anunciar sua existência, informar suas capacidades, ser configurado, entrar em operação, receber comandos, publicar estados e gerar eventos sem que o Host precise conhecer previamente a implementação específica de cada sensor ou atuador.

```text
                     ┌───────────────┐
                     │    Clients    │
                     └───────┬───────┘
                             │
                            MQTT
                             │
                     ┌───────▼───────┐
                     │    Broker     │
                     └───────┬───────┘
                             │
                     ┌───────▼───────┐
                     │  HostDevice   │
                     └───────┬───────┘
                             │
                    Serial / I²C / SPI
                             │
                     ┌───────▼───────┐
                     │  NodeControl  │
                     │               │
                     │ StateMachine  │
                     │ Protocol      │
                     │ Monitors      │
                     │ NodeController│
                     └───────┬───────┘
                             │
                 ┌───────────┼───────────┐
                 ▼           ▼           ▼
              Sensors     Actuators    Triggers
```

The project is currently under active development.
