library(ggplot2)
library(data.table)

Training <- data.table::fread("Training.csv")

Training = melt(Training, id.vars = "iter")

ggplot(Training, ) +
  geom_line(aes(x = iter, y = value)) +
  #scale_y_log10() + 
  facet_wrap(~variable, scales = "free")

