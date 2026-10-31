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
im,d=canvas(340);label(d,(35,15),'size = 4, capacity = 4')
for i,x in enumerate([5,2,7,1]):box(d,40+i*100,70,90,60,str(x))
arrow(d,(470,100),(650,100));label(d,(680,15),'push_back(9): capacity = 8')
for i in range(8):box(d,680+i*85,70,80,60,str([5,2,7,1,9][i]) if i<5 else '', '#fff0d9' if i==4 else gray)
label(d,(45,200),'Копируем 4 старых элемента; записываем 1 новый.',32)
label(d,(45,255),'Логический порядок сохранён. Ссылки на старый массив недействительны.',27);save(im,'vector')
im,d=canvas(380);label(d,(35,15),'До переноса: in (снизу вверх)')
for i,x in enumerate([5,2,7]):box(d,50+i*110,85,95,65,str(x))
label(d,(55,180),'out пуст');arrow(d,(430,115),(650,115));label(d,(720,15),'После переноса: out (снизу вверх)')
for i,x in enumerate([7,2,5]):box(d,750+i*110,85,95,65,str(x))
label(d,(940,175),'вершина: 5',28);label(d,(55,260),'Очередь слева направо: 5, 2, 7. Новый push(1) идёт в in.',30)
label(d,(55,315),'minimum = min(minimum(in), minimum(out))',30);save(im,'queue')
im,d=canvas(550)
levels=[[(600,20,200,'[0,8): 36')],[(260,150,220,'[0,4): 14'),(950,150,220,'[4,8): 22')],[(80,290,210,'[0,2): 9'),(410,290,210,'[2,4): 5'),(760,290,210,'[4,6): 9'),(1090,290,220,'[6,8): 13')]]
for child in levels[1]:arrow(d,(700,90),(child[0]+child[2]/2,150))
for i,child in enumerate(levels[2]):par=levels[1][i//2];arrow(d,(par[0]+par[2]/2,220),(child[0]+child[2]/2,290))
for level in levels:
 for x,y,w,t in level:box(d,x,y,w,70,t,'#fff0d9' if t.startswith(('[2,4)','[4,6)')) else gray)
arrow(d,(1200,360),(1220,405));box(d,1110,405,220,65,'[6,7): 5','#fff0d9')
label(d,(40,490),'sum(2,7) = 5 + 9 + 5 = 19; выбранные диапазоны не пересекаются.',28);save(im,'segment')
im,d=canvas(440);box(d,470,25,460,80,'[0,4): sum = 26, add = 3','#fff0d9')
arrow(d,(580,105),(350,220));arrow(d,(820,105),(1050,220))
box(d,120,220,460,85,'[0,2): 9 → 15');box(d,820,220,460,85,'[2,4): 5 → 11')
label(d,(60,355),'push: каждый ребёнок получает +3 на элемент; метка родителя очищается.',28)
label(d,(60,395),'После передачи: 15 + 11 = 26. Сумма родителя не меняется.',28);save(im,'lazy')
im,d=canvas(330)
box(d,50,40,150,70,'x = 2');arrow(d,(220,75),(340,75));box(d,360,40,320,70,'set(5), add(3)');arrow(d,(700,75),(820,75));box(d,850,40,350,70,'результат 8','#fff0d9')
box(d,50,190,150,70,'x = 2');arrow(d,(220,225),(340,225));box(d,360,190,320,70,'add(3), set(5)');arrow(d,(700,225),(820,225));box(d,850,190,350,70,'результат 5','#fff0d9');save(im,'composition')
im,d=canvas(350);label(d,(40,20),'Учебная ширина слова w = 8; универсум 0…19')
for word in range(3):
 for bit in range(8):
  x=word*8+bit
  box(d,40+word*450+bit*49,100,45,65,str(x) if x<20 else '—','#fff0d9' if x in [13,16] else gray)
 label(d,(40+word*450,195),'слово '+str(word),28)
label(d,(40,270),'13 = 1 · 8 + 5; 16 = 2 · 8 + 0. Биты 20…23 обязаны быть нулевыми.',29);save(im,'bits')
im,d=canvas(370)
for i,(t,x) in enumerate([('L₀ = {0}',50),('L₁ = {1,2}',380),('L₂ = {3}',760),('L₃ = {4}',1100)]):
 box(d,x,60,250,80,t)
 if i<3:arrow(d,(x+260,100),(x+315,100))
label(d,(40,205),'next = объединение adj[v] для v из текущего слоя, затем next \\ seen',28)
label(d,(40,265),'Ребро 3 → 1 не возвращает вершину 1: она уже посещена.',29)
label(d,(40,320),'Расстояния: [0, 1, 1, 2, 3].',29);save(im,'bfs')

# Textbook sections 5-7.
im,d=canvas(350)
label(d,(40,15),'B = 4; запрос [2,11)',30)
values=[2,7,1,4,3,6,5,8,9,0,2,1]
for i,v in enumerate(values):
 x=40+i*108
 box(d,x,85,96,65,str(v),'#fff0d9' if 2<=i<11 else gray)
 label(d,(x+35,160),str(i),23)
for x,t in [(40,'сумма 14'),(472,'сумма 22'),(904,'сумма 12')]:label(d,(x+80,225),t,28)
label(d,(40,285),'1 + 4  |  22  |  9 + 0 + 2  =  38',30);save(im,'blocks')
im,d=canvas(360)
box(d,60,50,410,90,'L: [1, 1]');box(d,920,50,410,90,'R: [2, 3]')
label(d,(70,175),'суммы: 0, 1, 1, 2',30);label(d,(930,175),'суммы: 0, 2, 3, 5',30)
arrow(d,(490,95),(890,95));label(d,(540,35),'T − сумма слева',30)
label(d,(60,265),'T = 3: пары (0,3), (1,2), (1,2). Ответ 3; кратности сохраняем.',30);save(im,'mitm')
im,d=canvas(400)
box(d,55,40,260,75,'r3: вершина 7');box(d,55,235,260,75,'r5: вершина 9')
box(d,535,135,250,80,'узел 2');box(d,975,135,250,80,'узел 5')
arrow(d,(330,80),(515,165));arrow(d,(330,270),(515,185));arrow(d,(805,175),(955,175))
label(d,(550,245),'общий хвост',30);label(d,(65,340),'Узлы неизменяемы. r4 = pop(r2) указывает прямо на узел 5.',30);save(im,'versions')
im,d=canvas(360)
box(d,40,125,370,85,'v: front пуст, back длины n')
for y,t in [(30,'pop(v) → ветвь 1'),(135,'pop(v) → ветвь 2'),(240,'pop(v) → ветвь m')]:
 box(d,800,y,500,70,t);arrow(d,(430,165),(780,y+35))
label(d,(40,285),'Каждая ветвь повторяет разворот: всего Θ(mn) работы.',29);save(im,'persistent-queue')
