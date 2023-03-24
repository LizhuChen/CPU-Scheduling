# CPU-Scheduling
CPU執行的模擬(FCFS、NSJF、PSJF、RR、PP)

共分成6種case，分別為執行FCFS、NSJF、PSJF、RR、PP以及前五種都執行
- FCFS為先到先處理，依抵達時間進行先後執行
- NSJF則依CPU Burst最短的先進行處理，並且為不可奪取
- PSJF和NSJF不一樣的地方在可奪取，也就是當等待佇列中出現比正在執行之process的CPU Burst更小時，會搶走CPU進行執行
- RR則依抵達時間小的進行處理，但因有時間片段的機制，若執行完時間片段但還未執行完，仍須先上給下一個process使用
- PP則以priority小的先執行經由五種不同的執行方式後

將waiting time 和turnned around time還有Gantt chart一起輸出到output檔
