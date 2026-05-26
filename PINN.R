library(ggplot2)
library(data.table)

PINN <- data.table::fread("PINN.csv")
f = function(t) {
  1.0 / (1.0 + exp(-t))
  #sin(t)
}

points_x = c(-2.01499, -0.927784, 0.918228, 1.3557, 0.636374, -1.77536, -0.146351, -1.6312, 0.398675, -0.559446)
point_sy = f(points_x)

ggplot(PINN, aes(x = time)) +
  geom_line(data = PINN, aes(y = NN, group = labda, col = factor(labda))) +
  geom_function(fun = f) + 
  geom_point(data = data.frame(time = points_x, y = point_sy), aes(y = y)) + 
  theme_test() + 
  ylab("Value") + xlab("Time") + 
  xlim(c(-10, 10))+ ylim(c(-1.0, 2.5)) + 
  guides(col = guide_legend("Predictor")) + 
  ggtitle("PINN vs NN", "f' = f * (1 - f)")

