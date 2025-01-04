filename = "C:\\Users\\Niels\\Documents\\PhD\\Nilib\\ModelTraining.log"
data = read.csv("C:\\Users\\Niels\\Documents\\PhD\\Nilib\\ModelTraining.log")
require(ggplot2)

ggplot(data, aes(x=epoch)) + 
  geom_line(aes(y=avg_loss, col="Training Loss")) + 
  geom_line(aes(y=val_loss, col="Validation Loss")) + 
  geom_ribbon(aes(y=avg_loss, ymin = avg_loss - std_loss, ymax = avg_loss + std_loss ), alpha = 0.2) + 
  geom_ribbon(aes(y=val_loss, ymin = val_loss - std_vall_loss, ymax = val_loss + std_vall_loss ), alpha = 0.2)
