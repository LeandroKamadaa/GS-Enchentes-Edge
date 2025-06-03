# 💧 ResQ - Sistema de Monitoramento de Enchentes com IoT

## Integrantes


## 📌 Descrição do Problema

Inundações urbanas causam prejuízos econômicos, riscos à vida e desinformação da população. A ausência de um monitoramento em tempo real impede ações preventivas e respostas rápidas por parte da comunidade e autoridades.

## 💡 Visão Geral da Solução

O **ResQ** é uma solução baseada em IoT que detecta riscos de alagamento utilizando sensores de nível d’água (HC-SR04), temperatura e umidade (DHT22). O sistema envia dados em tempo real via **MQTT** para um broker público, podendo ser consumido por dashboards, sistemas públicos ou aplicações móveis.

A visualização local é feita por um display LCD e LEDs indicadores, alertando imediatamente sobre o risco com **níveis de alerta visuais e sonoros**.

## 🧠 Tecnologias Utilizadas

- **ESP32**  
- **Sensor Ultrassônico HC-SR04** – Para medir a distância da água  
- **Sensor DHT22** – Temperatura e umidade do ar  
- **Display LCD 16x2 com I2C**  
- **LEDs (verde, amarelo, vermelho)** – Níveis de alerta  
- **Buzzer** – Alerta sonoro em caso de risco crítico  
- **Wi-Fi (Wokwi-GUEST)**  
- **MQTT (broker: test.mosquitto.org)**  
- **Protocolo JSON com ArduinoJson**  
- **Wokwi (Simulador Virtual)**

## 🚦 Níveis de Alerta

| LED         | Situação     | Critério                      |
|-------------|--------------|-------------------------------|
| 🟢 Verde     | Seguro       | Distância ≥ 70cm              |
| 🟡 Amarelo   | Alerta       | 30cm ≤ Distância < 70cm       |
| 🔴 Vermelho  | Perigo       | Distância < 30cm + Buzzer     |

## 🧪 Instruções de Execução e Simulação

1. **Acesse o projeto no Wokwi:**

   👉 [Simular no Wokwi](https://wokwi.com/projects/432691845362368513)

2. **Configuração dos Componentes:**

   - DHT22 no GPIO 15  
   - TRIG do HC-SR04 no GPIO 5  
   - ECHO do HC-SR04 no GPIO 18  
   - LED Verde no GPIO 22  
   - LED Amarelo no GPIO 21  
   - LED Vermelho no GPIO 19  
   - Buzzer no GPIO 23  
   - LCD I2C: SDA no GPIO 13, SCL no GPIO 12 (endereço 0x27)

3. **Broker MQTT:**

   - Host: `test.mosquitto.org`  
   - Porta: `1883`  
   - Tópico: `leandro/1espb/enchente`

4. **Visualização dos dados:**

   - Serial Monitor do Wokwi
   - Display LCD (tempo real)
   - Broker MQTT (payload em JSON)

## 🔧 Exemplo de Payload Publicado

```json
{
  "temperatura": 25.6,
  "umidade": 58.2,
  "distancia": 42,
  "status": "Alerta"
}
