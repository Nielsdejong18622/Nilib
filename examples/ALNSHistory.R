require(data.table)
require(ggplot2)

ALNShistory = data.table::fread("ALNS.csv", stringsAsFactors = T)
ALNShistory[, Iteration := 1:.N, Thread]
ALNShistory[, BestFound := cummin(Objective), Thread]
ALNShistory$Thread = factor(ALNShistory$Thread)

ggplot(ALNShistory[]) + 
  geom_point(aes(x = Iteration, y = AnnealProb, col = Thread)) + 
  #geom_line(aes(x = Iteration, y = BestFound, col = Thread), linetype = "dashed") + 
  theme_test() + 
  ggtitle("ALNS History")


ggplot(ALNShistory[Iteration < 5000]) + 
  geom_line(aes(x = Iteration, y = Objective, col = Thread)) + 
  geom_line(aes(x = Iteration, y = BestFound, col = Thread), linetype = "dashed") + 
  theme_test() + 
  ggtitle("ALNS History")



ALNShistory[, .(min(Objective), .N), Thread]

