import sqlite3
import os
import sys

def main(args):

    task_id =0

    if not os.path.isfile("world.db"):
        db = open("world.db", "w+")

        dbcon = sqlite3.connect('world.db')

        with dbcon:
            cursor = dbcon.cursor()
            cursor.execute("CREATE TABLE tasks("
                           "id INT PRIMARY KEY, "
                           "name TEXT NOT NULL, "
                           "worker_id INT, "
                           "time_to_make INT, "
                           "resource_name TEXT NOT NULL, "
                           "resource_amount INT, "
                           "FOREIGN KEY(worker_id) REFERENCES workers(id), "
                           "FOREIGN KEY(resource_name) REFERENCES resources(name)"
                           ")")

            cursor.execute("CREATE TABLE workers("
                           "id INT PRIMARY KEY, "
                           "name TEXT NOT NULL, "
                           "status TEXT NOT NULL" 
                           ")")

            cursor.execute("CREATE TABLE resources("
                           "name TEXT NOT NULL PRIMARY KEY, "
                           "amount INT"
                           ")")

            input = args[1]
            with open(input) as input:
                for line in input:
                    count = 0
                    for char in line:
                        if char == ",":
                            count = count +1

                    if count==1: #resource
                        pos1 = line.find(",")
                        cursor.execute("INSERT INTO resources "
                                       "VALUES(?,?)", (line[0:pos1], line[pos1+1:],))
                    if count==2: #worker
                        pos1 = line.find(",")
                        pos2 = line.rfind(",")
                        if line[len(line)-1:] == "\n":
                            pos3 = len(line)-1
                        else:
                            pos3 = len(line)
                        cursor.execute("INSERT INTO workers "
                                       "VALUES(?,?,?)", (line[pos1+1:pos2], line[pos2 + 1:pos3], "idle",))

                    if count==4: #task
                        pos1 = line.find(",")
                        pos2 = pos1 + line[pos1+1:].find(",") +1
                        pos3 = pos2 + line[pos2+1:].find(",") +1
                        pos4 = line.rfind(",")
                        task_id= task_id +1

                        cursor.execute("INSERT INTO tasks "
                                       "VALUES(?, ?, ?, ?, ?, ?)", (task_id,
                                                                    line[0:pos1],
                                                                    line[pos1+1:pos2],
                                                                    line[pos4+1:],
                                                                    line[pos2+1:pos3],
                                                                    line[pos3+1:pos4],
                                                                    ))


if __name__ == '__main__':
    main(sys.argv)