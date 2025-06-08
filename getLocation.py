import requests as rq
import pandas as pd
from time import sleep
from account import apiKey
import subprocess
import sys

#global settings:
user_agent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36'
header = {'User-Agent': user_agent}
url = 'https://maps.googleapis.com/maps/api/geocode/json'
input_file = "address.csv"
output_file = "location.csv"
columns = ("店舗名", "住所", "lat", "lng")

def getLocation(address):
    params = {'address': address, 'key': apiKey}
    response = rq.get(url, headers=header, params=params)
    data = response.json()
    if data['status'] == 'OK':
        location = data['results'][0]['geometry']['location']
        return location['lat'], location['lng']
    else:
        print(f"住所変換失敗: {address}, ステータス: {data['status']}")
        return None, None

if __name__ == "__main__":
    data = pd.read_csv(input_file).to_dict(orient='list')
    results = {column: [] for column in columns}
    print("\nconversion start.\n")
    for name, address in zip(data[columns[0]], data[columns[1]]):
        result = {column: None for column in columns}
        lat, lng = getLocation(address)

        if lat is not None and lng is not None:
            result[columns[0]] = name
            result[columns[1]] = address
            result[columns[2]] = lat
            result[columns[3]] = lng
            for column in columns:
                results[column].append(result[column])
            if len(results[columns[0]]) % 10 == 0:
                print(len(results[columns[0]]), "/", len(data[columns[0]]))
        sleep(0.11)

    print("\nconversion completed.\n")
    odf = pd.DataFrame(results)
    odf.to_csv(output_file, encoding="utf-8", mode="w", index=False)