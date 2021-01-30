import serial
import tkinter as tk
from time import sleep

arduino = serial.Serial('COM4', 9600)


def key_handler(key):
    print(bytes(key, 'ascii'))
    arduino.write(bytes(key, 'ascii'))


if __name__ == '__main__':
    root = tk.Tk()
    root.bind("<Key>", lambda e: key_handler(e.char))
    tk.Grid.rowconfigure(root, 0, weight=1)
    tk.Grid.columnconfigure(root, 0, weight=1)

    main = tk.Frame(root)
    tk.Grid.rowconfigure(main, 0, weight=1)
    tk.Grid.rowconfigure(main, 1, weight=0)
    tk.Grid.columnconfigure(main, [0, 2], weight=1)
    tk.Grid.columnconfigure(main, 1, weight=2)

    left_main = tk.Frame(main)
    tk.Grid.columnconfigure(left_main, 0, weight=1)
    tk.Grid.rowconfigure(left_main, 0, weight=1)

    mid_main = tk.Frame(main)
    tk.Grid.columnconfigure(mid_main, 0, weight=1)
    tk.Grid.rowconfigure(mid_main, [0, 1], weight=1)

    right_main = tk.Frame(main)
    tk.Grid.columnconfigure(right_main, 0, weight=1)
    tk.Grid.rowconfigure(right_main, 0, weight=1)

    forward_button = tk.Button(mid_main, text="FORWARD", command=lambda: key_handler('w'))
    left_button = tk.Button(left_main, text="TURN LEFT", command=lambda: key_handler('a'))
    back_button = tk.Button(mid_main, text="BACKWARD", command=lambda: key_handler('s'))
    right_button = tk.Button(right_main, text="TURN RIGHT", command=lambda: key_handler('d'))
    stop_button = tk.Button(main, text="STOP", command=lambda: key_handler('e'))
    # play button
    # pause button

    main.grid(column=0, row=0, sticky=tk.NSEW)
    left_main.grid(column=0, row=0, sticky=tk.NSEW)
    mid_main.grid(column=1, row=0, sticky=tk.NSEW)
    right_main.grid(column=2, row=0, sticky=tk.NSEW)

    left_button.grid(column=0, row=0, sticky=tk.NSEW)
    forward_button.grid(column=0, row=0, sticky=tk.NSEW)
    back_button.grid(column=0, row=1, sticky=tk.NSEW)
    right_button.grid(column=0, row=0, sticky=tk.NSEW)
    stop_button.grid(column=0, row=1, sticky=tk.EW)

    tk.mainloop()
