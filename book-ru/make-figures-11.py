from pathlib import Path
from PIL import Image, ImageDraw, ImageFont
P=Path(__file__).resolve().parent/'figures'
FONT='C:/Windows/Fonts/arial.ttf'
blue='#245981'; orange='#c77b2a'; gray='#e9f0f5'
def canvas(h=360):
    im=Image.new('RGB',(1400,h),'white');return im,ImageDraw.Draw(im)
def label(d,xy,t,size=30,color=blue):
    d.text(xy,t,font=ImageFont.truetype(FONT,size),fill=color)
def box(d,x,y,w,h,t,fill=gray):
    d.rounded_rectangle((x,y,x+w,y+h),radius=9,fill=fill,outline=blue,width=3)
    font=ImageFont.truetype(FONT,28);bounds=d.textbbox((0,0),t,font=font)
    d.text((x+(w-bounds[2])/2,y+(h-(bounds[3]-bounds[1]))/2-5),t,font=font,fill=blue)
def arrow(d,a,b):
    d.line((a,b),fill=orange,width=5)
    import math
    angle=math.atan2(b[1]-a[1],b[0]-a[0])
    d.polygon([b,(b[0]-17*math.cos(angle-.4),b[1]-17*math.sin(angle-.4)),(b[0]-17*math.cos(angle+.4),b[1]-17*math.sin(angle+.4))],fill=orange)
def save(im,name):im.save(P/(name+'.png'))

im,d=canvas(480)
label(d,(35,10),'Поиск lower_bound(45): 20 → 40 → ответ 50',32)
keys=[10,20,30,40,50,60]; heights=[1,3,1,2,1,3]
for level in (2,1,0):
 y=75+(2-level)*110
 box(d,45,y,135,60,'head')
 positions=[45]
 for j,key in enumerate(keys):
  if heights[j]>level:
   x=240+j*185
   box(d,x,y,125,60,str(key),'#fff0d9' if key==50 and level==0 else gray)
   positions.append(x)
 for j in range(len(positions)-1):
  start=positions[j]+(135 if j==0 else 125)+5
  arrow(d,(start,y+30),(positions[j+1]-7,y+30))
label(d,(40,395),'Высоты: 10 → 1; 20 → 3; 30 → 1; 40 → 2; 50 → 1; 60 → 3.',29)
label(d,(40,440),'На каждом уровне пропускаем только ключи меньше 45.',29)
save(im,'skip-list')
im,d=canvas(420)
label(d,(40,15),'Слияние без старых снимков: новые версии ключей',32)
label(d,(40,80),'Старый файл',30);label(d,(520,80),'Новый файл',30);label(d,(1000,80),'После слияния',30)
for x,rows in [(40,['A = 10, t = 1','B = 20, t = 2']),
               (520,['A = 15, t = 3','B = DEL, t = 4']),
               (1000,['A = 15, t = 3','B = DEL, t = 4'])]:
 for j,t in enumerate(rows):box(d,x,140+j*95,350,65,t,'#fff0d9' if x==1000 else gray)
arrow(d,(400,172),(505,172));arrow(d,(880,172),(985,172))
label(d,(40,340),'Текущее чтение: A = 15, B отсутствует.',30)
label(d,(40,385),'Метка DEL не удаляется, пока она нужна для скрытия старой записи.',28)
save(im,'lsm-merge')
