#Collect script info
initial.options <- commandArgs(trailingOnly = F) 
file.arg.name <- "--file="
script.name <- sub(file.arg.name, "", initial.options[grep(file.arg.name, initial.options)])
script.basename <- dirname(script.name)

library('getopt')

#input params
#0=no argument required
#1=argument required
#2=argument optional
spec = matrix(c(
'skiplines', 's', 2, "integer",
'index' , 'i', 2, "integer",
'pvalue' , 'p', 2, "integer",
'oddsratio' , 'o', 2, "integer",
'beta' , 'b', 2, "integer",
'standarderror' , 'e', 2, "integer",
'Nindividuals' , 'n', 2, "integer",
'zscore' , 'z', 2, "integer",
'allelefreq' , 'a', 2, "integer",
'functionfile' , 'f', 2, "character",
'help' , 'h', 0, "logical"
), byrow=TRUE, ncol=4)

opt = getopt(spec)
# if help was asked for print a friendly message
# and exit with a non-zero error code
if ( !is.null(opt$help) ) {
  cat(getopt(spec, usage=TRUE))
  q(status=1)
}

# set some reasonable defaults for the options that are needed,
# but were not specified.
if ( is.null(opt$skiplines) ) { opt$skiplines = 0 }

# Read functions in functionfile
if(is.null(opt$functionfile)){
  cat("functionfile needs to be specified")
  q(status=1)
}else{
  funcs <- scan(opt$functionfile, what="character", quiet=TRUE)
}

# Make inxs vector for argument positions
argcolvals <- rep(NA, length=7)
if(!is.null(opt$pvalue)){argcolvals[1] <- opt$pvalue}
if(!is.null(opt$oddsratio)){argcolvals[2] <- opt$oddsratio}
if(!is.null(opt$beta)){argcolvals[3] <- opt$beta}
if(!is.null(opt$standarderror)){argcolvals[4] <- opt$standarderror}
if(!is.null(opt$Nindividuals)){argcolvals[5] <- opt$Nindividuals}
if(!is.null(opt$zscore)){argcolvals[6] <- opt$zscore}
if(!is.null(opt$allelefreq)){argcolvals[7] <- opt$allelefreq}

#source functions (by first setting workdirectory to same as where the main script was called from)
source(file.path(script.basename,"functions.R"))

f <- file("stdin")
open(f)

#remove header
if (!is.null(opt$skiplines)) { 
  skip <- readLines(f,n=opt$skiplines)
}

while(length(line <- readLines(f,n=1)) > 0) {
  #split line on tab
  strsp <- strsplit(line, split="\t")
  arr <- unlist(strsp)

 #use index if present and print output of first func
 if (!is.null(opt$index)) { 
   cat(arr[opt$index],"\t")
   do.call(funcs[1], list(arr=arr,inxs=argcolvals)) 
 }else{
   do.call(funcs[1], list(arr=arr,inxs=argcolvals)) 
 }
  #for remaining funcs
  for (func in funcs[2:length(funcs)]){
    cat("\t")
    do.call(func, list(arr=arr,inxs=argcolvals)) 
  }
  cat("\n")
}

# signal success and exit
q(status=0)

