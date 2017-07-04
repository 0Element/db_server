#!/usr/bin/env python2.7
from cassandra.cluster import Cluster
import time
import threading


def cassandra_conn():
    cluster = Cluster(['127.0.0.1'])
    session = cluster.connect();
    session.execute("USE test_keyspace")

    return session


def cassandra_exec(session, i):
    session.execute("""INSERT INTO users (user_name, state, birth_year)
        VALUES (%s, %s, %s)""", ("John OReilly" + str(i), "man", 1942))


def cassandra_each(session):
    rows = session.execute('SELECT user_name, state, birth_year FROM users')

    i = 0
    for user_row in rows:
        print user_row.user_name#, user_row.state, user_row.birth_year
        i+=1
    print "count: ", i


def thr_exec(count, thr):
    t0 = round(time.time(), 0)
    tmp_t = t0
    tmp_i = 0

    session = cassandra_conn()
    for i in xrange(count):
        cassandra_exec(session, i + (count * thr - count))

        tmp_t = round(time.time(), 0)
        if t0 != tmp_t:
            print i - tmp_i, " : ", thr, "\n"
            tmp_i = i
            t0 = tmp_t


if __name__ == "__main__":
    count = 1000 * 1000 * 10
    thrs = 1

    #for i in xrange(thrs):
    #    thr = threading.Thread(target=thr_exec, args=(count, i))
    #    thr.start()

    session = cassandra_conn()
    cassandra_each(session)

    print "END!"
