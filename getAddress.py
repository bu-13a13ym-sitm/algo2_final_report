import requests as rq
from bs4 import BeautifulSoup as bs
import urllib.parse
import pandas as pd
from concurrent.futures import ThreadPoolExecutor as th
from time import sleep
from random import randrange
import subprocess
import sys

#global settings:
user_agent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36'
header = {'User-Agent': user_agent}
base_url = "https://r.gnavi.co.jp/area/jp/rs/?"
output_file = r"address.csv"
timeout = 10
columns = ("店舗名", "住所")

def randomWait():
    sleep(randrange(1, 3))

def getSoup(url):
    response = rq.get(url, headers=header, timeout=timeout)
    randomWait()

    response.raise_for_status()
    response.encoding = response.apparent_encoding
    soup = bs(response.content, "html.parser")

    return soup

def getDetails(url):
    results = []
    try:
        page = getSoup(url)
        result = {column: None for column in columns}
        result[columns[0]] = page.find("div", id="header-wrapper")\
                                .find("h1")\
                                .get_text(strip=True)\
                                .replace("\u00a0", "")\
                                .replace("\xa0", "")\
                                .replace("\u3000", " ")
        table = page.find("div", id="info-table")
        result[columns[1]] = table.find("span", class_="region")\
                                .get_text(strip=True)
        results.append(result)
    except Exception as e:
        print(f"Error in getDetails: {e}")

    return results

def getInfos(resultUrl):
    urls = []
    results = []
    try:
        soup = getSoup(resultUrl)
        restaurants = soup.find("div", class_="style_resultRestaurant__Oa8cF")
        for url in restaurants.find_all("a", class_="style_titleLink___TtTO"):
            urls.append(url["href"])
        
        results = []
        with th(max_workers=5) as executer:
            pageResults = executer.map(getDetails, urls)
            for result in pageResults:
                results.extend(result)
    except Exception as e:
        print(f"Error in getInfos: {e}")

    return results

if __name__ == "__main__":
    print("input search keywords:")
    search_keywords = {
        "fw": input("genre: "),
        "fwp": input("prefecture: ")
    }
    numSearch = int(input("number of search results: "))
    query = urllib.parse.urlencode(search_keywords)
    resultUrl = base_url + query
    resultUrls = [f"{resultUrl}&p={i}" for i in range(1, numSearch // 30 + 1 if numSearch % 30 == 0 else numSearch // 30 + 2)]
    results = {column: [] for column in columns}

    print("\nscraping start.\n")
    fin = False
    with th(max_workers=5) as executer:
        page_results = executer.map(getInfos, resultUrls)
        for page in page_results:
            for shop in page:
                for column in columns:
                    results[column].append(shop.get(column, None))
                if len(results[columns[0]]) % 10 == 0:
                    print(len(results[columns[0]]), "/", numSearch)
                if len(results[columns[0]]) >= numSearch:
                    fin = True
                    break
            if fin:
                break
    
    print("\nscraping completed.\n")
    df = pd.DataFrame(results)
    df.to_csv(output_file, encoding="utf-8", mode="w", index=False)
    subprocess.run([sys.executable, "./getLocation.py"])
    subprocess.run([sys.executable, "./showLocation.py", "location.csv"])
    subprocess.run(["./main.exe", "location.csv"])
    subprocess.run([sys.executable, "./showLocation.py", "location.csv", "graham_outline.csv"])
    subprocess.run([sys.executable, "./showLocation.py", "location.csv", "andrew_outline.csv"])