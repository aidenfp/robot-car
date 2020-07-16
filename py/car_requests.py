import sqlite3
import ast

paths_db = '__HOME__/car/path.db'


def request_handler(request):
    conn = sqlite3.connect(paths_db)
    c = conn.cursor()
    create_database()
    if request['method'] == 'POST':
        if len(c.execute('''SELECT * from path_table;''').fetchall()) != 0:
            c.execute('''DELETE FROM path_table;''')
            conn.commit()
            conn.close()
        insert_path(request['form']['data'])
        return "Path posted!"
    else:
        try:
            point = c.execute('''SELECT * from path_table ORDER BY path_ind ASC;''').fetchone()
            ind = point[0]
            c.execute('''DELETE FROM path_table WHERE path_ind = ?;''', (ind,))
            conn.commit()
            return point[1]
        except:
            return "NO PATH"

def create_database():
    conn = sqlite3.connect(paths_db)
    c = conn.cursor()
    c.execute('''CREATE TABLE IF NOT EXISTS path_table (path_ind int, point text);''')
    conn.commit()
    conn.close()


def insert_point(data):
    conn = sqlite3.connect(paths_db)
    c = conn.cursor()
    ind = len(c.execute('''SELECT * from path_table;''').fetchall())
    c.execute('''INSERT into path_table VALUES(?,?);''', (ind, data))
    conn.commit()
    conn.close()


def insert_path(data_string):
    path = ast.literal_eval(data_string)
    for point in path:
        insert_point(str(point))
