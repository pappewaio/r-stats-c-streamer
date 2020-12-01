pval_oddsratio_2_zscore <- function(arr,inxs){
  pval <- as.numeric(arr[inxs[1]])
  or <- as.numeric(arr[inxs[2]])
  cat(sign(or)*abs(qnorm(pval/2)))
}

pval_beta_2_zscore <- function(arr,inxs){
  pval <- as.numeric(arr[inxs[1]])
  beta <- as.numeric(arr[inxs[3]])
  cat(sign(beta)*abs(qnorm(pval/2)))
}

pval_beta_N_2_zscore <- function(arr,inxs){
  pval <- as.numeric(arr[inxs[1]])
  beta <- as.numeric(arr[inxs[3]])
  N <- as.integer(arr[inxs[5]])
  cat(abs(qt(pval/2, N-2))*sign(beta))
}

beta_se_2_zscore <- function(arr,inxs){
  se <- as.numeric(arr[inxs[4]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(beta/se)
}

zscore_N_2_pvalue <- function(arr,inxs){
  N <- as.integer(arr[inxs[5]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(2*pt(-abs(zscore), N-2 ))
}

zscore_se_2_beta <- function(arr,inxs){
  se <- as.numeric(arr[inxs[4]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(zscore*se)
}

pval_qnorm <- function(arr,inxs){
  pval <- as.numeric(arr[inxs[1]])
  cat(qnorm(pval/2))
}

zscore_2_pvalue <- function(arr,inxs){
  zscore <- as.numeric(arr[inxs[6]])
  cat(2*pnorm(-abs(zscore)))
}

zscore_N_af_2_beta <- function(arr,inxs){
  zscore <- as.numeric(arr[inxs[6]])
  N <- as.integer(arr[inxs[5]])
  afreq <- as.integer(arr[inxs[7]])
  cat(1/sqrt(2*afreq*(1-afreq)*(N+zscore^2)))
}

zscore_beta_2_se <- function(arr,inxs){
  beta <- as.numeric(arr[inxs[3]])
  zscore <- as.numeric(arr[inxs[6]])
  cat(beta/zscore)
}

zscore_N_af_2_se <- function(arr,inxs){
  zscore <- as.numeric(arr[inxs[6]])
  N <- as.integer(arr[inxs[5]])
  afreq <- as.integer(arr[inxs[7]])
  cat(1/sqrt(2*afreq*(1-afreq)*(N + zscore^2)))
}

zscore_beta_af_2_N <- function(arr,inxs){
  zscore <- as.numeric(arr[inxs[6]])
  beta <- as.numeric(arr[inxs[3]])
  afreq <- as.integer(arr[inxs[7]])
  cat(((zscore^2)/(2*afreq*(1-afreq)*beta^2))-zscore^2)
}

