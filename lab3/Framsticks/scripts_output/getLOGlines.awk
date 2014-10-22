#fiters out lines with the "[LOG]" token. Useful to process output from standard.expdef with logging activated
{
  x=index($0,"[LOG]"); 
  if (x>0) print substr($0,x+6);
}