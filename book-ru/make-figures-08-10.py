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

im,d=canvas(500)
label(d,(40,10),'Старый корень                         Новый корень: set(1, 7)',32)
box(d,100,80,260,65,'[0,4): 0');box(d,780,80,260,65,'[0,4): 7','#fff0d9')
box(d,30,245,250,65,'[0,2): 0');box(d,560,245,250,65,'[2,4): 0')
box(d,1010,245,250,65,'[0,2): 7','#fff0d9')
arrow(d,(180,145),(155,245));arrow(d,(300,145),(620,245))
arrow(d,(840,145),(750,245));arrow(d,(1000,145),(1120,245))
box(d,885,395,210,60,'[0,1): 0');box(d,1150,395,210,60,'[1,2): 7','#fff0d9')
arrow(d,(1080,310),(990,395));arrow(d,(1190,310),(1250,395))
label(d,(40,400),'Оранжевый: новые узлы.',28)
label(d,(40,445),'Голубой: общие нулевые диапазоны.',28)
save(im,'persistent-segment')
im,d=canvas(500)
label(d,(40,15),'BST: вставка 4',32);label(d,(720,15),'Левосторонняя куча: ранги',32)
box(d,200,80,170,60,'5 → 5′','#fff0d9');box(d,60,240,170,60,'2 → 2′','#fff0d9')
box(d,390,240,170,60,'8');box(d,240,390,170,60,'4','#fff0d9')
arrow(d,(240,140),(140,240));arrow(d,(330,140),(470,240));arrow(d,(190,300),(300,390))
box(d,920,80,200,60,'2, rank 2');box(d,720,240,200,60,'5, rank 1');box(d,1130,240,200,60,'8, rank 1')
arrow(d,(970,140),(820,240));arrow(d,(1070,140),(1230,240))
label(d,(700,380),'rank(left) ≥ rank(right)',30)
label(d,(700,435),'rank(node) = rank(right) + 1',30);save(im,'persistent-trees')
im,d=canvas(450)
label(d,(40,10),'Частичная персистентность: коробка заполнена',32)
box(d,70,110,370,80,'старый узел + история');box(d,820,110,470,80,'копия текущего состояния','#fff0d9')
arrow(d,(465,150),(790,150));label(d,(490,80),'копирование',27)
box(d,70,300,370,65,'старые корни');box(d,820,300,470,65,'текущие родители')
arrow(d,(250,300),(250,195));arrow(d,(1050,300),(1050,195))
label(d,(40,400),'Исторические ссылки сохраняются; перенаправляются только текущие.',29);save(im,'node-copying')
im,d=canvas(490)
box(d,210,30,220,60,'v0: x = 0');box(d,50,195,220,60,'v1: x = 5','#fff0d9')
box(d,420,195,220,60,'v2: x = 0');box(d,50,365,220,60,'v3: x = 5','#fff0d9')
arrow(d,(270,90),(160,195));arrow(d,(380,90),(530,195));arrow(d,(160,255),(160,365))
label(d,(730,45),'DFS: v0, v1, v3, v2',32)
label(d,(730,125),'v0: [0,4)',30);label(d,(730,195),'v1: [1,3)',30)
label(d,(730,265),'v3: [2,3)',30);label(d,(730,335),'v2: [3,4)',30)
label(d,(720,420),'v1 не является предком v2.',29);save(im,'version-euler')
