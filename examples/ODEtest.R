library(readr)
library(ggplot2)
require(plotly)

ODEtest <- read_csv("../simpleODE.csv")
predatorPrey = read_csv("../predatorprey.csv")
lorentz = read_csv("../lorentz.csv")

ggplot(data = ODEtest) + 
  geom_line(aes(x = idx, y= realval, col = "Actual")) + 
  geom_line(aes(x = idx, y= ODEval, col = "ODE")) + 
  theme_minimal()

ggplot(data = predatorPrey) + 
  geom_line(aes(x = time, y= prey, col = "Prey")) + 
  geom_line(aes(x = time, y= predator, col = "Predator")) + 
  theme_minimal()

plot_ly(
  data=lorentz, x = ~x, y = ~y, z = ~z,
  type = 'scatter3d', mode = 'lines',
  opacity = 1, line = list(width = 6, color = ~color, reverscale = FALSE)
)