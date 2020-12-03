zscore_from_pval_oddsratio <- function(arr,inxs){
  pval <- as.numeric(arr[inxs[1]])
  or <- as.numeric(arr[inxs[2]])
  cat(sign(or)*abs(qnorm(pval/2)))
}

zscore_from_pval_beta <- function(arr,inxs){
  pval <- as.numeric(arr[inxs[1]])
  beta <- as.numeric(arr[inxs[3]])
  cat(sign(beta)*abs(qnorm(pval/2)))
}

zscore_from_pval_beta_N <- function(arr,inxs){
  pval <- as.numeric(arr[inxs[1]])
  beta <- as.numeric(arr[inxs[3]])
  N <- as.integer(arr[inxs[5]])
  cat(abs(qt(pval/2, N-2))*sign(beta))
}

zscore_from_beta_se <- function(arr,inxs){
  se <- as.numeric(arr[inxs[4]])
  beta <- as.numeric(arr[inxs[3]])
  cat(beta/se)
}

pval_from_zscore_N <- function(arr,inxs){
  N <- as.integer(arr[inxs[5]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(2*pt(-abs(zscore), N-2 ))
}

pval_from_zscore <- function(arr,inxs){
  zscore <- as.numeric(arr[inxs[6]])
  cat(2*pnorm(-abs(zscore)))
}

pval_qnorm <- function(arr,inxs){
  pval <- as.numeric(arr[inxs[1]])
  cat(qnorm(pval/2))
}

beta_from_zscore_N_af <- function(arr,inxs){
  zscore <- as.numeric(arr[inxs[6]])
  N <- as.integer(arr[inxs[5]])
  afreq <- as.numeric(arr[inxs[7]])
  cat(zscore/sqrt(2*afreq*(1-afreq)*(N+zscore^2)))
}

beta_from_zscore_se <- function(arr,inxs){
  se <- as.numeric(arr[inxs[4]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(zscore*se)
}

se_from_zscore_beta <- function(arr,inxs){
  beta <- as.numeric(arr[inxs[3]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(beta/zscore)
}

se_from_zscore_N_af <- function(arr,inxs){
  zscore <- as.numeric(arr[inxs[6]])
  N <- as.integer(arr[inxs[5]])
  afreq <- as.numeric(arr[inxs[7]])
  cat(1/sqrt(2*afreq*(1-afreq)*(N + zscore^2)))
}

N_from_zscore_beta_af <- function(arr,inxs){
  zscore <- as.numeric(arr[inxs[6]])
  beta <- as.numeric(arr[inxs[3]])
  afreq <- as.numeric(arr[inxs[7]])
  cat(((zscore^2)/(2*afreq*(1-afreq)*beta^2))-zscore^2)
}

