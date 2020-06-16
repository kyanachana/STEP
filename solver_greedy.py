#!/usr/bin/env python3

import sys
import math
import csv
import random

from common import print_tour, read_input
#greedyから交差をなくした

def distance(city1, city2):
    return math.sqrt((city1[0] - city2[0]) ** 2 + (city1[1] - city2[1]) ** 2)

def intersect_check(citya,cityb,cityc,cityd):#２線分が交差してるか確認する
    xa = citya[0]
    ya = citya[1]
    xb = cityb[0]
    yb = cityb[1]
    xc = cityc[0]
    yc = cityc[1]
    xd = cityd[0]
    yd = cityd[1]
    s = (xa-xb)*(yc-ya)-(ya-yb)*(xc-xa)
    t = (xa-xb)*(yd-ya)-(ya-yb)*(xd-xa)
    u = (xc-xd)*(ya-yc)-(yc-yd)*(xa-xc)
    v = (xc-xd)*(yb-yc)-(yc-yd)*(xb-xc)
    if(s*t<0 and u*v<0):
        return 1
    return 0

def intersect_erase(tour,cities,current_num,next_num):
    
    for i in range(len(tour)-1):           
        if(intersect_check(cities[tour[i]],cities[tour[i+1]],cities[tour[current_num]],cities[tour[next_num]])):

            if i>current_num:
                first_begin = current_num
                first_end = next_num
                second_begin = i
                second_end = i+1
            elif i<current_num:
                first_begin = i
                first_end = i+1
                second_begin = current_num
                second_end = next_num
            
            #交差した２本の内、最初に訪れる線分をfirst、２番めに訪れる線分をsecondにする
            #beginは線分のうちの入ってくる方の点、endは出ていく方の点
            
            tour[first_end],tour[second_begin]=tour[second_begin],tour[first_end]
            #first_endとsecond_beginを入れ替える

            for j in range(1,int((second_begin-first_end-1)/2)+1):
                #入れ替えたことでfirst_endからsecond_beginまでの点の流れを逆にする必要がある
                tour[first_end+j],tour[second_begin-j]=tour[second_begin-j],tour[first_end+j]
            

            #再帰で新たに出来た２本についても交差が生じているか確認する
            before_second_begin = tour[second_begin]
            before_second_end = tour[second_end]
            intersect_erase(tour,cities,first_begin,first_end)

            #一本目の交差解消の際にsecond_beginとsecond_endに何番目に訪れるかが変化してしまうので対応する
            for j in range(len(tour)):
                if tour[j]==before_second_begin:
                    second_begin = j
                elif tour[j]==before_second_end:
                    second_end = j
            if second_begin>second_end:
                second_begin,second_end=second_end,second_begin
            
            intersect_erase(tour,cities,second_begin,second_end) 

            return

    return
            
   
    
def solve(cities,whole_start):
    N =len(cities) 

    dist = [[0] * N for i in range(N)]
    for i in range(N):
        for j in range(i, N):
            dist[i][j] = dist[j][i] = distance(cities[i], cities[j])
    
    current_city = whole_start
    unvisited_cities = set(range(0, N))
    tour = [current_city]
    unvisited_cities.remove(current_city)

    while unvisited_cities:
        next_city = min(unvisited_cities,key=lambda city: dist[current_city][city])
        unvisited_cities.remove(next_city)
        tour.append(next_city)
        
        intersect_erase(tour,cities,len(tour)-2,len(tour)-1)
        #点を打つたびにその点と１つ前の点でできる線分と他の線分が交差しているか確認する。
        #交差していたら交差しないように入れ替える
        
        current_city = tour[len(tour)-1]
        
    tour.append(whole_start)
    intersect_erase(tour,cities,len(tour)-2,len(tour)-1)
    #最後に打った点とその次の点(0の点)についても同じく交差している線分があるか確認する。
    #del(tour[len(tour)-1])
    total_distance = 0
    for i in range(len(tour)):
        if i == len(tour)-1:
            total_distance += distance(cities[tour[i]],cities[0])
        else:
            total_distance += distance(cities[tour[i]],cities[tour[i+1]])
    return tour,total_distance


if __name__ == '__main__':
    assert len(sys.argv) > 1
    cities = read_input(sys.argv[1])
    
    min_total_distance = 10000000
    min_pattern_num = -1
    for i in range(10):#ケース５まではなんとかいけるがケース６の場合は計算量が多すぎる
        tour,total_distance = solve(cities,i)
        if min_total_distance>total_distance:
           min_total_distance=total_distance 
           min_pattern_num = i
    #tour,total_distance = solve(cities,0)#ケース６の場合
    print(min_total_distance)
    tour,total_distance = solve(cities,min_pattern_num)
    print(total_distance)
    print_tour(tour)
    with open('output_2.csv', 'w')as file: #change the file name every time
        writer = csv.writer(file)
        writer.writerow(["index"])
        for i in range(len(tour)):
            writer.writerow([tour[i]])
        file.close()


