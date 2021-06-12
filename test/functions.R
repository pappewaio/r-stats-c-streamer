
#####
#
# Linear regression
#
#####

lin_zscore_from_pval_beta <- function(arr,inxs, mods){
  pval <- as.numeric(arr[inxs[1]])
  beta <- as.numeric(arr[inxs[3]])
  if (mods[2] == 1) {
    pval = 10^-pval;
  }
  cat(sign(beta)*abs(qnorm(pval/2)))
}

lin_zscore_from_pval_beta_N <- function(arr,inxs, mods){
  pval <- as.numeric(arr[inxs[1]])
  beta <- as.numeric(arr[inxs[3]])
  N <- as.integer(arr[inxs[5]])
  if (mods[2] == 1) {
    pval = 10^-pval;
  }
  cat(abs(qt(pval/2, N-2))*sign(beta))
}

lin_zscore_from_beta_se <- function(arr,inxs, mods){
  se <- as.numeric(arr[inxs[4]])
  beta <- as.numeric(arr[inxs[3]])
  cat(beta/se)
}

lin_pval_from_zscore_N <- function(arr,inxs, mods){
  N <- as.integer(arr[inxs[5]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(2*pt(-abs(zscore), N-2 ))
}

lin_pval_from_zscore <- function(arr,inxs, mods){
  zscore <- as.numeric(arr[inxs[6]])
  cat(2*pnorm(-abs(zscore)))
}

lin_pval_qnorm <- function(arr,inxs, mods){
  pval <- as.numeric(arr[inxs[1]])
  if (mods[2] == 1) {
    pval = 10^-pval;
  }
  cat(qnorm(pval/2))
}

lin_beta_from_zscore_N_af <- function(arr,inxs, mods){
  zscore <- as.numeric(arr[inxs[6]])
  N <- as.integer(arr[inxs[5]])
  afreq <- as.numeric(arr[inxs[7]])
  if (mods[2] == 1) {
    afreq = 1-afreq;
  }
  cat(zscore/sqrt(2*afreq*(1-afreq)*(N+zscore^2)))
}

lin_beta_from_zscore_se <- function(arr,inxs, mods){
  se <- as.numeric(arr[inxs[4]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(zscore*se)
}

lin_se_from_zscore_beta <- function(arr,inxs, mods){
  beta <- as.numeric(arr[inxs[3]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(beta/zscore)
}

lin_se_from_zscore_N_af <- function(arr,inxs, mods){
  zscore <- as.numeric(arr[inxs[6]])
  N <- as.integer(arr[inxs[5]])
  afreq <- as.numeric(arr[inxs[7]])
  if (mods[2] == 1) {
    afreq = 1-afreq;
  }
  cat(1/sqrt(2*afreq*(1-afreq)*(N + zscore^2)))
}

lin_N_from_zscore_beta_af <- function(arr,inxs, mods){
  zscore <- as.numeric(arr[inxs[6]])
  beta <- as.numeric(arr[inxs[3]])
  afreq <- as.numeric(arr[inxs[7]])
  if (mods[2] == 1) {
    afreq = 1-afreq;
  }
  cat(((zscore^2)/(2*afreq*(1-afreq)*beta^2))-zscore^2)
}

#####
#
# Logistic regression
#
#####

######
##
## Stat conversion formulas to be implemented in sumstats cleaner
##
## Could be interesting to study the effects of compounded inference - is there bias?
## For example, Only P and B are recorded.  Get Z_fromP, use Z_fromP to get SE_fromBZ.
##				Compare SE from B,Z to SE from B,Z_fromP
##
## We should be aware of different models and if conversions are valid
##
## 2p(1-p), the variance of a SNP genotypes, is a crucial quantity that is typically underestimated
##	because it assumed HWE.  This is unlikely to be true in principal ever for a case-control trait, 
##	but especially if there is population stratification or cryptic relatedness.  See comparisons where 
##	it is an estimate that is typically biased downwards relative to the true SNP variance.
##
## Further sensitivity testing and studies of downstream consequences of mis-conversions would be 
##	interesting to pursue.  Espcially in the context of very subtle pop strat and the potential 
##	detection of and correction of inflated effect sizes that could be done per SNP rather than a 
##	global correction that is the same for all SNPs.
##
######

log_zscore_from_beta_se <- function(arr,inxs, mods){
  beta <- as.numeric(arr[inxs[3]])
  se <- as.numeric(arr[inxs[4]])
  cat(beta/se)
}

log_zscore_from_pval_oddsratio <- function(arr,inxs, mods){
  pval <- as.numeric(arr[inxs[1]])
  or <- as.numeric(arr[inxs[2]])
  if (mods[2] == 1) {
    pval = 10^-pval;
  }
  cat(sign(or)*abs(qnorm(pval/2)))
}

log_pval_from_zscore <- function(arr,inxs, mods){
  zscore <- as.numeric(arr[inxs[6]])
  cat(2*pnorm(-abs(zscore)))
} 

log_beta_from_oddsratio <- function(arr,inxs, mods){
  or <- as.numeric(arr[inxs[2]])
  cat(log( or ))
}

log_beta_from_zscore_se <- function(arr,inxs, mods){
  se <- as.numeric(arr[inxs[4]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(zscore * se)
}

log_se_from_beta_zscore <- function(arr,inxs, mods){
  beta <- as.numeric(arr[inxs[3]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(beta / zscore)
}

log_se_from_ORu95_ORl95 <- function(arr,inxs, mods){
  or_u95 <- as.numeric(arr[inxs[8]])
  or_l95 <- as.numeric(arr[inxs[9]])
  cat( (log(or_u95) - log(or_l95)) / ( 2*qnorm( 0.975 )) )
}

log_Neff_from_Nca_Nco <- function(arr,inxs, mods){
  ncases <- as.numeric(arr[inxs[10]])
  ncontrols <- as.numeric(arr[inxs[11]])
  cat(4 / ( (1/ncases) + (1/ncontrols) ))
}

