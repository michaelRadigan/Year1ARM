EXTRA_SRC =
TEX = pdflatex

all: Report.pdf Checkpoint.pdf Presentation.pdf

.phony: all clean

%.pdf: %.tex $(EXTRA_SRC)
	while( $(TEX) $< ; grep -q "Rerun to get cross" $(<:.tex=.log)) ; \
	  do true ; \
	 done

clean:
	rm -f *.aux
	rm -f *.out
	rm -f *.nav
	rm -f *.dvi
	rm -f *.snm
	rm -f *.toc
	rm -f *.gz
	rm -f *.vrb
	rm -f *.log

  
