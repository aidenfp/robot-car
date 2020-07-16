import turtle
import requests
from turtle import Turtle, Screen


def polar_diff(point, prev=(0, 0, 0)):
    x1, y1, h1 = prev
    x2, y2, h2 = point
    dx = x2 - x1
    dy = y2 - y1
    r = round((dx ** 2 + dy ** 2) ** (1 / 2), 2)
    theta = h2-h1
    if 3.14 < abs(theta) <= 6.28:
        theta -= 6.28*abs(theta)/theta

    return r, round(-theta, 2)


class DataTurtle(Turtle):

    def __init__(self):
        Turtle.__init__(self)
        self.data = []
        self.path = []

    def to_path(self):
        for ind in range(len(self.data)):
            if ind != 0:
                ppoint = polar_diff(self.data[ind], self.data[ind - 1])
                self.path.append(ppoint)
            else:
                self.path.append(polar_diff(self.data[ind]))
        return self.path

    def clear(self):
        Turtle.clear(self)
        self.data = []


screen = Screen()
t = DataTurtle()
turtle.setup()
t.speed(-1)
t.pensize(5)
t.shape("arrow")
t.radians()


def draw(x, y):
    t.ondrag(None)
    t.setheading(t.towards(x, y))
    t.goto(x, y)
    t.data.append((x, y, round(t.heading(), 2)))
    t.ondrag(draw)


def click_right(x, y):
    t.up()
    t.home()
    t.clear()
    t.down()


def set_location(x, y):
    t.setheading(t.towards(x, y))
    t.forward(t.distance(x, y))
    t.data.append((x, y, round(t.heading(), 2)))


def show_turtle(x, y):
    t.showturtle()


def main():
    turtle.listen()
    t.ondrag(draw)
    turtle.onscreenclick(set_location, 1)
    turtle.onscreenclick(click_right, 3)
    t.onrelease(show_turtle)
    screen.mainloop()
    return t.to_path()


if __name__ == '__main__':
    string = str(main())
    print(string)
    print(requests.post("http://608dev-2.net/sandbox/sc/aidenfp/car/car_requests.py", data={'data': string}).text)
    # print(requests.get("http://608dev-2.net/sandbox/sc/aidenfp/car/car_requests.py").text)
