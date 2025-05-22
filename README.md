check list
D1 : O
D2 : O
D3 : O
D4 : O
D5 : O

구현 사항
1. score중(week, UZ) 하나라도 max score에 도달하면 즉시 reset score. 
   (week와 UZ 모두 증가해야하는 상황에서 week가 먼저 max에 도달했다면 UZ 10으로 reset 후 증가.)

2. reset score이후 score가 10 아래로 내려간 경우 0.1배 증가해도 score가 증가하지 못하므로 score 증가 최소값은 1로함
   (score를 증가시킬 때 증가량이 1 미만이라면 1증가)