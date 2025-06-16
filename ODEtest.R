

library(readr)
ODEtest <- read_csv("ODEtest.csv")

library(ggplot2)
ggplot(data = ODEtest) + 
  geom_line(aes(x = idx, y= realval, col = "Actual")) + 
  geom_line(aes(x = idx, y= ODEval, col = "ODE")) + 
  theme_minimal()
