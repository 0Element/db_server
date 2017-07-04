import threading
import time
import os


class Plugins(object):
    def __init__(self, dir_plugs, time_update):
        self.dir_plugs = dir_plugs
        self.time_update = time_update
        self.quit = False
        self.mods = []

        t = threading.Thread(target=self.reload)
        t.setDaemon(True)
        t.start()
        print("Constructor Plugins")


    def reload(self):
        while(not self.quit):
            files = [f for f in os.listdir(self.dir_plugs) if f.startswith("plug_")]
            for f in files:
                f = f[:-3]
                print(f)
                mod = getattr(getattr(__import__("plug." + f), f), "Plugin")
                #print(dir(mod))
                self.mods.append(mod)

            print("reload plugins")
            time.sleep(self.time_update)


    def __del__(self):
        print("Destructor Plugins")