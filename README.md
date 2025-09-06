# Introdução à Engenharia Mecatrônica – IoT Industrial

Este repositório é destinado aos estudantes de graduação em **Engenharia Mecatrônica** da **Universidade Federal de Uberlândia (UFU)** e contém materiais didáticos, exemplos de código e projetos práticos relacionados à disciplina **FEMEC-33101 – Introdução à Engenharia Mecatrônica**.

## 📚 Conteúdo Abordado

Os materiais presentes neste repositório cobrem os seguintes tópicos:

### 1. **Introdução à IoT e Protocolo MQTT**
- Conceitos fundamentais de Internet das Coisas (IoT) e sua aplicação em mecatrônica.
- Protocolo MQTT e sua arquitetura *publish/subscribe*.
- Qualidades de Serviço (QoS) e componentes do MQTT.

### 2. **ESP32: Hardware e Programação**
- Introdução ao microcontrolador ESP32.
- Características técnicas, pinagem e variantes.
- Configuração do ambiente de desenvolvimento com Arduino IDE.
- Instalação de bibliotecas essenciais (PubSubClient, EWiFi, CLPIoT, PN532).

### 3. **Programação Básica e Avançada**
- Estrutura de sketches Arduino.
- Controle de GPIO, comunicação serial e temporização não-bloqueante.
- Técnicas de debug e logging profissional.

### 4. **Comunicação MQTT com ESP32**
- Conexão Wi-Fi doméstica e empresarial usando a biblioteca `EWiFi`.
- Implementação de cliente MQTT com `PubSubClient`.
- Exemplo completo de sistema de monitoramento e controle remoto.

### 5. **Exercícios Práticos**
- **Exercício 1**: Controle remoto de LED via MQTT usando aplicativo Android.
- **Exercício 2**: Sistema de controle de acesso com RFID e comunicação MQTT.

## 🛠️ Tecnologias Utilizadas
- **Microcontrolador**: ESP32 (DevKit V1)
- **IDE**: Arduino IDE
- **Protocolo**: MQTT
- **Bibliotecas**: PubSubClient, EWiFi, LiquidCrystal, PN532
- **Broker MQTT**: Teste com broker público (ex: `test.mosquitto.org`)

## 📂 Estrutura do Repositório
O repositório está organizado em pastas por assunto, incluindo:
- Códigos-fonte exemplos (`*.ino`)
- Bibliotecas personalizadas
- Esquemas de conexão
- Documentação complementar

## 👨‍🎓 Público-Alvo
Estudantes de Engenharia Mecatrônica que desejam aprender na prática sobre:
- Integração hardware-software
- Comunicação sem fio
- Sistemas embarcados e IoT industrial
- Automação e controle remoto
