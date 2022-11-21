# TRABALHO DO GRAU B - UNISINOS
# Disciplina: Processamento Gráfico
# Alunos: Gabriela Mezetti e Pedro Brandelli

import cv2 as cv
import tkinter
from tkinter import *
from tkinter import filedialog
import numpy as np
from PIL import Image, ImageTk
import filters


def mostrar_imagem(img, label, cvt=False):
    global param, background, eh_video
    img = cv.resize(img, (800, 800))
    param = cvt
    if cvt:
        if cvt == 'blur':
            img = cv.GaussianBlur(img, (21, 21), 0)
        elif cvt == 'sepia':
            img = filters.sepia(img)
        elif cvt == 'negativo':
            img = cv.bitwise_not(img)
        elif cvt == 'contorno':
            img = cv.Canny(cv.GaussianBlur(img, (5, 5), 0), 50, 100)
        elif cvt == 'nitido':
            img = filters.nitidez(img)
        elif cvt == 'lapis_peb':
            img = filters.lapis(img, True)
        elif cvt == 'lapis_cor':
            img = filters.lapis(img, False)
        elif cvt == 'verao':
            img = filters.verao(img)
        elif cvt == 'inverno':
            img = filters.inverno(img)

        else:
            img = cv.cvtColor(img, cvt)

    img = cv.cvtColor(img, cv.COLOR_BGR2RGB)

    if eh_video:
        img = cv.flip(img, 1)

    pil_img = Image.fromarray(img)
    tk_img = ImageTk.PhotoImage(pil_img)
    label.configure(image=tk_img)
    label.image = tk_img
    label.place(x=200, y=0)
    background = pil_img


def mostrar_sticker(stk, label):
    global background, sticker, sticker_video, sticker_x, sticker_y, final_img
    img = cv.imread('stickers/'+stk, cv.IMREAD_UNCHANGED)
    alpha = np.ones((120, 120)) * 255
    if img.shape[2] == 4:
        _, _, _, alpha = cv.split(img)
        alpha = cv.resize(alpha, (120, 120))

    img = cv.resize(img, (120, 120))
    img = cv.cvtColor(img, cv.COLOR_BGR2BGRA)
    img[:, :, 3] = alpha

    img = cv.cvtColor(img, cv.COLOR_BGRA2RGBA)

    pil_img = Image.fromarray(img, mode='RGBA')
    bg = background.copy()
    bg.paste(pil_img, (sticker_x, sticker_y), pil_img)
    tk_img = ImageTk.PhotoImage(bg)
    label.configure(image=tk_img)
    label.image = tk_img
    label.place(x=200, y=0)
    sticker = stk
    if eh_video:
        sticker_video = sticker
    final_img = bg.copy()


def video():
    global imagem, param
    _, imagem = cap.read()
    mostrar_imagem(imagem, lbl, param)
    if sticker_video:
        mostrar_sticker(sticker_video, lbl)
    lbl.after(1, video)


def check_radio(x):
    global imagem, file, eh_video
    if x == 1:
        file = filedialog.askopenfilename(initialdir='/')
        imagem = cv.imread(file)
        mostrar_imagem(imagem, lbl)
        eh_video = False
    else:
        eh_video = True
        video()


def key_pressed(event):
    global sticker, sticker_x, sticker_y
    if event.keysym == "Right":
        sticker_x += 5
        mostrar_sticker(sticker, lbl)
    elif event.keysym == "Left":
        sticker_x -= 5
        mostrar_sticker(sticker, lbl)
    elif event.keysym == "Up":
        sticker_y -= 5
        mostrar_sticker(sticker, lbl)
    elif event.keysym == "Down":
        sticker_y += 5
        mostrar_sticker(sticker, lbl)


def salvar():
    global background, final_img
    if sticker:
        final_img.save(filedialog.asksaveasfilename())
    else:
        background.save(filedialog.asksaveasfilename())


def limpar(imagem, lbl, param):
    global sticker_video, eh_video
    if eh_video:
        sticker_video = False
    else:
        mostrar_imagem(imagem, lbl, param)


win = Tk()
win.geometry('1000x900')
win.title('Trabalho GB')
win.bind("<Key>", key_pressed)

radio = IntVar()
cap = cv.VideoCapture(0)
global imagem, file, param, background, sticker, sticker_video, eh_video, sticker_x, sticker_y, final_img

lbl = Label(win)

param = False
sticker = False
sticker_video = False
eh_video = False

sticker_x, sticker_y = 350, 350

titulo = tkinter.Label(win, text="TRABALHO DO GB \n PROCESSAMENTO GRÁFICO", font='Helvetica 10 bold')
titulo.place(x=10, y=10)

inst = tkinter.Label(win, text="Selecione a opção desejada:", font='Helvetica 9 bold')
inst.place(x=10, y=55)

R1 = Radiobutton(win, text='Foto', font='Helvetica 9 bold', variable=radio, value=1, command=lambda: check_radio(1))
R1.place(x=10, y=75)
R2 = Radiobutton(win, text='Vídeo', font='Helvetica 9 bold', variable=radio, value=2, command=lambda: check_radio(2))
R2.place(x=100, y=75)

content = tkinter.Frame(win)


botao0 = tkinter.Button(win, text='IMAGEM ORIGINAL', command=lambda: mostrar_imagem(imagem, lbl))
botao0.place(x=10, y=150)


fil = tkinter.Label(win, text="Filtros:", font='Helvetica 9 bold')
fil.place(x=10, y=225)

botao1 = tkinter.Button(win, text='Grayscale', command=lambda: mostrar_imagem(imagem, lbl, cv.COLOR_BGR2GRAY))
botao1.place(x=10, y=250)

botao2 = tkinter.Button(win, text='Blur', command=lambda: mostrar_imagem(imagem, lbl, 'blur'))
botao2.place(x=10, y=300)

botao3 = tkinter.Button(win, text='Sépia', command=lambda: mostrar_imagem(imagem, lbl, 'sepia'))
botao3.place(x=10, y=350)

botao4 = tkinter.Button(win, text='Negativo', command=lambda: mostrar_imagem(imagem, lbl, 'negativo'))
botao4.place(x=10, y=400)

botao5 = tkinter.Button(win, text='Contorno', command=lambda: mostrar_imagem(imagem, lbl, 'contorno'))
botao5.place(x=10, y=450)

botao7 = tkinter.Button(win, text='Nítido', command=lambda: mostrar_imagem(imagem, lbl, 'nitido'))
botao7.place(x=10, y=500)

botao8 = tkinter.Button(win, text='Lápis P&B', command=lambda: mostrar_imagem(imagem, lbl, 'lapis_peb'))
botao8.place(x=10, y=550)

botao9 = tkinter.Button(win, text='Lápis Colorido', command=lambda: mostrar_imagem(imagem, lbl, 'lapis_cor'))
botao9.place(x=10, y=600)

botao10 = tkinter.Button(win, text='Verão', command=lambda: mostrar_imagem(imagem, lbl, 'verao'))
botao10.place(x=10, y=650)

botao11 = tkinter.Button(win, text='Inverno', command=lambda: mostrar_imagem(imagem, lbl, 'inverno'))
botao11.place(x=10, y=700)


st = tkinter.Label(win, text="Stickers:", font='Helvetica 9 bold')
st.place(x=300, y=825)

sticker_bt1 = tkinter.Button(win, text='Arco-Íris', command=lambda: mostrar_sticker('1.png', lbl))
sticker_bt1.place(x=400, y=825)

sticker_bt2 = tkinter.Button(win, text='Gato de Óculos', command=lambda: mostrar_sticker('2.png', lbl))
sticker_bt2.place(x=500, y=825)

sticker_bt1 = tkinter.Button(win, text='Doge', command=lambda: mostrar_sticker('3.png', lbl))
sticker_bt1.place(x=600, y=825)

sticker_bt2 = tkinter.Button(win, text='Cachorro', command=lambda: mostrar_sticker('4.png', lbl))
sticker_bt2.place(x=700, y=825)

sticker_bt1 = tkinter.Button(win, text='Gato', command=lambda: mostrar_sticker('5.png', lbl))
sticker_bt1.place(x=800, y=825)

clear = tkinter.Button(win, text='Limpar', font='Helvetica 9 bold', command=lambda: limpar(imagem, lbl, param))
clear.place(x=900, y=825)


save = tkinter.Button(win, text='SALVAR', font='Helvetica 9 bold', command=salvar)
save.place(x=10, y=800)


win.mainloop()
