import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.font_manager as fm
import platform
import sys

if platform.system() == 'Windows':
    font_path = 'C:\\Windows\\Fonts\\msgothic.ttc'
elif platform.system() == 'Darwin':
    font_path = '/System/Library/Fonts/ヒラギノ角ゴシック W3.ttc'
else:
    font_path = '/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc'

jp_font = fm.FontProperties(fname=font_path)

df = pd.read_csv(sys.argv[1])
lats = df["lat"]
lngs = df["lng"]

plt.figure(figsize=(10, 8))
plt.scatter(lngs, lats, c='red', alpha=0.7, edgecolors='k', s=80)
'''
if len(sys.argv) < 3:
    for i, name in enumerate(df["店舗名"]):
        plt.text(lngs[i], lats[i], name, fontsize=8, fontproperties=jp_font, ha='left', va='bottom')
'''
if len(sys.argv) >= 3:
    for path in sys.argv[2:]:
        df_line = pd.read_csv(path)
        line_lats = df_line["lat"]
        line_lats = list(line_lats) + [line_lats.iloc[0]]
        line_lngs = df_line["lng"]
        line_lngs = list(line_lngs) + [line_lngs.iloc[0]]
        plt.plot(line_lngs, line_lats, linestyle='-', marker='o')

plt.xlabel("Longitude (経度)", fontproperties=jp_font)
plt.ylabel("Latitude (緯度)", fontproperties=jp_font)
plt.title("焼肉店の位置プロット（神奈川県周辺）", fontproperties=jp_font)
plt.grid(True)
plt.tight_layout()
plt.show()
