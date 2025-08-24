library(ggplot2)
library(data.table)

BenchMark <- data.table::fread("Multiplication_bench.csv")

BenchMark = BenchMark[, .(avg_ns = mean(avg_ns), sd = sd(avg_ns)), by=name][order(avg_ns),]

BenchMark$name <- factor(BenchMark$name, levels = BenchMark$name[order(BenchMark$avg, decreasing = TRUE)])

ggplot(BenchMark, aes(x = name, y = avg_ns, fill = name,  group = name)) + 
  geom_bar(stat = "identity", position=position_dodge(), width = .2) +
  geom_errorbar(aes(ymin=avg_ns - sd, ymax=avg_ns +  sd), col = "grey", width=.3) + 
  theme_classic() + xlab("") + ylab("Average ns per benchmark") + 
  guides(fill = guide_legend(title = "Function")) + 
  scale_fill_viridis_d(option = "C") +
  ggtitle("Benchmark")