f = open("google_stock.csv","r")
data = []
next(f) #skipping first line containing column description
for line in f:
	data.insert(0,float(line.split(",")[4]))
fout = open("stock_to_plot.txt","w")
for item in data:
	fout.write("%s\n" % item)