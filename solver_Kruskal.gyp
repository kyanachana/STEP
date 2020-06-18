#!/usr/bin/env python3

import sys
import math
import csv
import numpy as np
from common import print_tour, read_input


def distance(city1, city2):
    return math.sqrt((city1[0] - city2[0]) ** 2 + (city1[1] - city2[1]) ** 2)

def root(a,parent):#find the root
    if a == parent[a]:
        return a
    else:
        parent[a] = root(parent[a],parent)
        return parent[a]

def is_same_root(a,b,parent):
    return root(a,parent)==root(b,parent)

def unite(a,b,parent):#connect one root to another root
    parent[root(a,parent)]=root(b,parent)
    return


def tour_distance(tour, dist): #calculate the whole distance
    length = 0
    for i in range(len(tour)):
        length += dist[tour[i]][tour[(i+1) % len(tour)]]
    return length


def reverse_segment(tour, i, j, dist):#reverse cities if the total distance is shortened
    a,b,c,d = tour[i-1], tour[i], tour[j-1], tour[j % len(tour)]
    if dist[a][b] + dist[c][d] > dist[a][c] + dist[b][d]:
        tour[i:j] = reversed(tour[i:j])


def alter_tour(tour, dist):
    before_length = tour_distance(tour, dist)
    for i in range(len(tour)):
        start = i
        for j in range(i+1,len(tour)):
            end = j
            reverse_segment(tour, start, end, dist)        
    if tour_distance(tour, dist) < before_length:
        alter_tour(tour, dist)
    return tour


def kruskal(cities,dist,used_way):
    N=len(cities)
    start_city = []
    end_city = []
    
    way =[]
    way_number = 0

    for i in range(N*N):
        #search all city to city distance(n*(n-1)/patterns)
        #way:store(dist,way_number)
        #start_city[way_number],end_city[way_number] get to know the cities from way_number
        a = int(i/N)
        b = i%N
        if a<b:
            start_city.append(a)
            end_city.append(b)
            way.append((dist[a][b],way_number))
            way_number+=1
   
    way.sort()

    parent = []
    for i in range(len(way)):
        parent.append(i)
    
    find_start = [0]*N #used to find the whole start

    for i in range(len(way)): #use ways in order of increasing dist
        num = way[i][1]
        start = start_city[num]
        end = end_city[num]

        if is_same_root(start,end,parent):  #avoid closed_path
            continue

        if find_start[start]==2 or find_start[end]==2: #a city can't be connected to more than three cities
            continue
        
        unite(start,end,parent)
        used_way.append((start,end))
        find_start[start]+=1
        find_start[end]+=1

    for i in range(N):
        if find_start[i]==1: #if a city is connected to only one city, the city is the whole_start or the whole_end
            whole_start = i
            break
    
    return whole_start


def find_route(tour,whole_start,N,used_way):
    tour.append(whole_start)
    current_city = whole_start
    for i in range(N-1):
        #if one edge is current_city, another edge is next_city
        for j in range(len(used_way)):
            if used_way[j][0]==current_city:
                current_city=used_way[j][1]
                del(used_way[j])
                tour.append(current_city)
                break
            elif used_way[j][1]==current_city:
                current_city=used_way[j][0]
                del(used_way[j])
                tour.append(current_city)
                break


def solve(cities):
    N =len(cities) 
    
    dist = [[0] * N for i in range(N)]
    for i in range(N):
        for j in range(i, N):
            dist[i][j] = dist[j][i] = distance(cities[i], cities[j])

    used_way=[]
    tour=[]

    whole_start = kruskal(cities,dist,used_way)
    find_route(tour,whole_start,N,used_way)   
    alter_tour(tour, dist) #improve the route using opt-2
    print(tour_distance(tour,dist))

    return tour



if __name__ == '__main__':
    assert len(sys.argv) > 1
    tour = solve(read_input(sys.argv[1]))
    print_tour(tour)
    with open('output_6.csv', 'w')as file: #change the file name every time
        writer = csv.writer(file)
        writer.writerow(["index"])
        for i in range(len(tour)):
            writer.writerow([tour[i]])
        file.close()