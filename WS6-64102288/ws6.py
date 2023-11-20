import paho.mqtt.client as mqtt
import requests
import json


def get_last_id():
    server_url = "http://192.168.2.41:3000/ws6"
    response = requests.get(server_url)
    data = response.json()

    last_id_list = [item['id'] for item in data if 'id' in item]
    last_id = max(last_id_list, default=0)
    return last_id


id_counter = 0


def on_connect(client, userdata, flags, rc):
    print("Connected to broker " + str(rc))
    client.subscribe("dht11")


def on_message(client, userdata, msg):
    global id_counter

    payload = msg.payload.decode()
    print(f"message on topic {msg.topic}: {payload}")

    try:
        data = json.loads(payload)

        last_id = get_last_id()
        data['id'] = last_id + 1

        print("Decoded JSON data:", data)
        server_url = "http://192.168.2.41:3000/ws6"

        response = requests.post(server_url, json=data)
        print(f"HTTP Response: {response.status_code}")

        if response.status_code == 200:
            print("Data sent successfully")
    except json.JSONDecodeError as e:
        print(f"Error decoding JSON: {e}")


client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.2.41", port=1883)

client.loop_forever()
