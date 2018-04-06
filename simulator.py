import os
import sqlite3

def main():
    check = os.path.isfile("world.db")
    if (check == True):
        dbcon = sqlite3.connect("world.db")

        with dbcon:
            cursor = dbcon.cursor()
            cursor.execute("SELECT * FROM tasks")
            taskslist = cursor.fetchall()
            if (taskslist == []):
                check = False

            while (check):
                for task in taskslist:
                    cursor.execute("SELECT worker_id FROM tasks WHERE id=(?)", (task[0],))
                    res1 = cursor.fetchone()  # worker_id
                    cursor.execute("SELECT status, name FROM workers WHERE id=(?)", (res1[0],))
                    res2 = cursor.fetchone()  # status,name
                    if (res2[0] == "busy"):
                        cursor.execute("SELECT id FROM tasks WHERE worker_id=(?)", (res1[0],))
                        res3 = cursor.fetchone() #taskid
                        if (res3[0] == task[0]):  # work on me

                            if (task[3] > 0):  # timetowork =0
                                cursor.execute("UPDATE tasks SET time_to_make=(?) "
                                               "WHERE id=(?)", (task[3] - 1, task[0],))
                                print(res2[1] + " is busy " + task[1] + "...")

                    else:  # idle
                        cursor.execute("UPDATE workers SET status=(?) "
                                       "WHERE id=(?)", ("busy", res1[0],))
                        print(res2[1] + " says: work work")

                    cursor.execute("SELECT * FROM tasks")
                    taskslist = cursor.fetchall()

                for task in taskslist:
                    cursor.execute("SELECT worker_id FROM tasks WHERE id=(?)", (task[0],))
                    res1 = cursor.fetchone()  # worker_id
                    cursor.execute("SELECT status, name FROM workers WHERE id=(?)", (res1[0],))
                    res2 = cursor.fetchone()  # status,name
                    if (task[3] == 0):
                        cursor.execute("DELETE FROM tasks "
                                       "WHERE id=(?)", (task[0],))

                        cursor.execute("UPDATE workers SET status=(?) "
                                       "WHERE id=(?)", ("idle", res1[0],))
                        print(res2[1] + " says: All Done!")


                check = os.path.isfile("world.db")
                if (check == True):
                    cursor.execute("SELECT * FROM tasks")
                    taskslist = cursor.fetchall()
                    if (taskslist == [] or taskslist==None):
                        check = False

if __name__ == '__main__':
    main()
