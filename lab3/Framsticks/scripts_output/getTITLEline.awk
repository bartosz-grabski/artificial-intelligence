#fiters out lines with the "[TITLE]" token. Useful to process output from standard.expdef with logging activated
{
  x=index($0,"[LOGTITLE]"); 
  if (x>0) print "set title \"" substr($0,x+11) "\"";
}