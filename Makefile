CC= gcc
TARGET= gsgraph
SOURCE= src/gs-misc.c src/gs-graph.c src/gs-plot.c src/gs-function-plot.c src/gs-dataset-plot.c src/dlgs/gs-plot-dialog.c src/gs-action-callbacks.c src/gs-plot-list-panel.c src/gs-window.c src/main.c


CFLAGS= `pkg-config --cflags --libs gtk+-2.0 libmatheval` -lm
all:
	$(CC) -o $(TARGET) $(CFLAGS) $(SOURCE) 
