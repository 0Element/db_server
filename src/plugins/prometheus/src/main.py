#!/usr/bin/env python2.7
from prometheus_client import start_http_server, Summary
import random
import time


# Create a metric to track time spent and requests made.
#REQUEST_TIME = Summary('request_processing_seconds', 'Time spent processing request')
REQUEST_TIME = Summary('request_MY2', 'Time MY2 request')

# Decorate function with metric.
@REQUEST_TIME.time()
def process_request(t):
    '''A dummy function that takes some time.'''
    time.sleep(t)

if __name__ == '__main__':
    # Start up the server to expose the metrics.
    start_http_server(8000)
    # Generate some requests.

    begin_t = time.time()
    t0 = round(begin_t, 0)
    tmp_t = t0
    tmp_i = 0

    for i in xrange(1000 * 1000 * 1):
        #process_request(random.random())
        process_request(0)

        tmp_t = round(time.time(), 0)
        if t0 != tmp_t:
            print i - tmp_i
            tmp_i = i
            t0 = tmp_t

    print time.time() - begin_t

    time.sleep(10)

#from prometheus_client.core import GaugeMetricFamily, CounterMetricFamily, REGISTRY

#class CustomCollector(object):
#    def collect(self):
#        yield GaugeMetricFamily('my_gauge', 'Help text', value=7)
#        c = CounterMetricFamily('my_counter_total', 'Help text', labels=['foo'])
#        c.add_metric(['bar'], 1.7)
#        c.add_metric(['baz'], 3.8)
#        yield c

#REGISTRY.register(CustomCollector())
