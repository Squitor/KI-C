import KI
import numpy as np
import timeit
"""
Player  1
Player -1
"""
#checks wether one player has won returns player or 0 if none

def check_winner(map):
    for i in range(3):
     if (map[i][0] == map[i][1] and map[i][1] == map[i][2]):
      return map[i][0]
    
    for i in range(3):
     if (map[0][i] == map[1][i] and map[1][i] == map[2][i]):
      return map[0][i]
     
    if (map[0][0] == map[1][1] and map[1][1] == map[2][2]):
     return map[0][0]
     
    if (map[0][2] == map[1][1] and map[1][1] == map[2][0]):
     return map[1][1]
    
    else:
     return 0

def print_map(map):
    print(map[0])
    print(map[1])
    print(map[2])

#converts map to array for ai

def conv_to_array(map, player):
    state = []
    for i in range(3):
     for j in range(3):
      if (map[i][j] == player):
       state.append(1)
      elif (map[i][j] == 0):
       state.append(0)
      else:
       state.append(-1)
    return state

def validate_play(play,map):
    if (map[int(play /3)][play %3] == 0):
     return 1
    else:
     return 0  

def find_valid_play(play,map):
    while 1:
     if (validate_play(np.argmax(play),map) == 0):
      play[np.argmax(play)] = 0
     else:
      return play

def playing(ki_1, ki_2,stable):
    map = [[0] * 3 for i in range(3)]
    i = 0
    while 1:
     play = KI.brain(stable[ki_1], conv_to_array(map,1))
     i = i + 1
     play = find_valid_play(play,map)
     map[int(np.argmax(play)/3)][np.argmax(play)%3] = 1
     if (not check_winner(map) == 0):
      return check_winner(map)
     if (i == 9):
      return 0

     play = KI.brain(stable[ki_2], conv_to_array(map,-1))
     i = i + 1
     play = find_valid_play(play,map)
     map[int(np.argmax(play)/3)][np.argmax(play)%3] = -1
     if (not check_winner(map) == 0):
      return check_winner(map)

def playing_human(ki_1,stable):
    map = [[0] * 3 for i in range(3)]
    i = 0
    while 1:
     play = KI.brain(stable[ki_1], conv_to_array(map,1))
     i = i + 1
     play = find_valid_play(play,map)
     map[int(np.argmax(play)/3)][np.argmax(play)%3] = 1
     if (not check_winner(map) == 0):
      return check_winner(map)
     if (i == 9):
      return 0
     
     print_map(map)
     play = int(input("Input: "))
     i = i + 1
     map[int(play/3)][play%3] = -1
     if (not check_winner(map) == 0):
      return check_winner(map)
 

Agents = 100
Generations = 1000
Trainer = 10  #x best networks play against the rest

stop = 1 # used for first timing because i am lazy
start = 1

stable = KI.Stable_init(Agents,9,9,7,9)
fit = [0]*Agents
for x in range(Generations):
 print("Playing Round ", x ,"of ", Generations, " Estimated Time: ", round((stop-start)*(Generations-x)/60, 1 ), "min")
 start = timeit.default_timer()
 for i in range(Trainer):
  for j in range(Agents):
   a = playing(i,j,stable)
   if (a == 1):
    fit[i] = fit[i] + 1
   if (a == -1):
    fit[j] = fit[j] + 1
 KI.Stable_Knecht(fit,stable,0.01,0.2,0.1)
 stop = timeit.default_timer()
KI.save_stable(stable)
playing_human(0,stable)




