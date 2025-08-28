library(ggplot2)
library(data.table)

Training <- data.table::fread("Training.csv")

Training = melt(Training, id.vars = "iter")

ggplot(Training[iter > 1000], ) +
  geom_line(aes(x = iter, y = value)) +
  geom_hline(yintercept = 0) + 
  #scale_y_log10() + 
  facet_wrap(~variable, scales = "free") + 
  theme_minimal()

