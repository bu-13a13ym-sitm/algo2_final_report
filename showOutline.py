import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.font_manager as fm
import platform
import sys

# 日本語フォントの設定（OSに応じて自動選択）
if platform.system() == 'Windows':
    font_path = 'C:\\Windows\\Fonts\\msgothic.ttc'
elif platform.system() == 'Darwin':  # macOS
    font_path = '/System/Library/Fonts/ヒラギノ角ゴシック W3.ttc'
else:  # Linux (例: Google Colab や Ubuntu)
    font_path = '/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc'

# フォント設定
jp_font = fm.FontProperties(fname=font_path)

# CSVファイル読み込み
df = pd.read_csv(sys.argv[1])
lats = df["lat"]
lats = list(lats) + [lats.iloc[0]]
lngs = df["lng"]
lngs = list(lngs) + [lngs.iloc[0]]

# プロット
plt.figure(figsize=(10, 8))
plt.scatter(lngs, lats, c='red', alpha=0.7, edgecolors='k', s=80)

# 各点を線で結ぶ（点の順に線を引く）
plt.plot(lngs, lats, linestyle='-', color='blue', linewidth=1)

for i in range(len(lats)):
    plt.text(lngs[i], lats[i], i + 1, fontsize=16, fontproperties=jp_font, ha='left', va='bottom')

# 軸・ラベル
plt.xlabel("Longitude (経度)", fontproperties=jp_font)
plt.ylabel("Latitude (緯度)", fontproperties=jp_font)
plt.title("焼肉店の位置凸包（神奈川県周辺）", fontproperties=jp_font)
plt.grid(True)
plt.tight_layout()
plt.show()
