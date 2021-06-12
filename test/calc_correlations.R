args <- commandArgs(trailingOnly = T)
#library(corrplot)

infile=args[1]
what=args[2]
what2=args[3]

tab <- read.table(infile, header=TRUE)[,-1]

header <- colnames(tab)
which_true <- grepl(paste("^",what,"$", sep=""), header)
which_inferred <- grepl(paste(what2,"_from", sep=""), header)
which_all <- which_true | which_inferred

#compare all against all 
if(sum(which_all) >1 ){
  tab2 <- tab[, which_all]

  #check if any have se==0
  sd <- apply(tab2,2,sd,na.rm=TRUE)
  sd0 <- sd==0

  #inform about any removed variables
  zeroset <- colnames(tab2)[sd0]
  for (cn in zeroset){
    message("message: Due to sd==0, not possible to use correlation for variable:", cn, "\n")
  }
  
  #remove from cor calculation and proceed if condition is met
  tab3 <- tab2[,!sd0, drop=FALSE]
  if(ncol(tab3)>1){
    cors <- cor(tab3)

    #print to screen (ok to loop as it will never be many rows)
    cat(paste("variable", paste(colnames(cors), collapse="\t"), sep="\t"), "\n")
    rn <- rownames(cors)
    for (i in 1:nrow(cors)){
      cat(paste(rn[i], paste(cors[1,], collapse="\t"), sep="\t"), "\n")
    }
  }else{
  cat("Not enough variables remain to compare for:", what, "\n")
  }
}else{
  #do nothing
  cat("Not enough variables to compare for:", what, "\n")
}

