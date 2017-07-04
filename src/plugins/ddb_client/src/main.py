#!/usr/bin/env python2.7

from ddbpy.client import Send
import time
import threading


def ddb_count(dfe, bucket, count):
    with Send(dfe) as send:
        send.switch_streaming(bucket)

        t0 = round(time.time(), 0)
        tmp_t = t0
        tmp_i = 0

        for i in xrange(count):
            # ts = int(time.time())
            value = "123.456"
            metric = "value"
            print send.send_payload(metric, ts, value)
            #time.sleep(1)

            tmp_t = round(time.time(), 0)
            if t0 != tmp_t:
                print i - tmp_i
                tmp_i = i
                t0 = tmp_t


if __name__ == '__main__':
    dfe = ('127.0.0.1', 5555)
    bucket = 'table1'
    count = 1000 * 1
    thrs = 1

    for i in xrange(thrs):
        thr = threading.Thread(target=ddb_count, args=(dfe, bucket, count))
        thr.start()
