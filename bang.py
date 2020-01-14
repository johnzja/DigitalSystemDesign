# import sys

fps = 30
listlen = 50


# now: int; mat50x7: 50*7 int; pretime: prenum*1 double; prerail: prenum*1 int; prenum: int; pins: 7*1 int
def main(now, mat50x7, pretime, prerail, prenum, pins):
    now = (now + 1) % listlen
    for i in list(prenum):
        preperiod = round(pretime * fps)
        mat50x7[(now + preperiod) % listlen][prerail[i]] = 1
        mat50x7[(now + preperiod + 1) % listlen][prerail[i]] = 1
        mat50x7[(now + preperiod + 2) % listlen][prerail[i]] = 1
        mat50x7[(now + preperiod + 3) % listlen][prerail[i]] = 1
    for i in list(7):
        pins[i] = mat50x7[now][i]
        mat50x7[now][i] = 0

