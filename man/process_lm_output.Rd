\name{process_lm_output}
\alias{process_lm_output}
\alias{process_simple_output}
\alias{sum_NA}
\alias{sum_not_NA}
\title{'apply2dfo'-associated functions}
\usage{
  process_lm_output(lmo,verbosity=2)
}
\arguments{
  \item{lmo}{object returned by analysis with "lm", "glm",
  etc.}

  \item{verbosity}{verbosity}
}
\description{
  A number of functions used in conjunction with
  'apply2dfo'. Standardly supported apply2dfo's anFUN
  analysis functions include 'lm', 'glm', 'coxph', 'sum',
  'prod', "sum_not_NA" (no. non-missing obs), and "sum_NA"
  (no. missing obs.). Pre-defined processing functions
  include "process_lm_output" (can process functions "lm",
  "glm", "coxph") and "process_simple_output" (process
  output from "sum", "prod", "sum_not_NA", "sum_NA")
}
\examples{
a <- matrix(rnorm(50),10,5)
rownames(a) <- paste("id",1:10,sep="")
colnames(a) <- paste("snp",1:5,sep="")
b <- as(a,"databel")
apply(a,FUN="sum",MAR=2)
apply2dfo(SNP,dfodata=b,anFUN="sum",procFUN="process_simple_output")
apply2dfo(SNP,dfodata=b,anFUN="sum",transpose=FALSE)

sex <- 1*(runif(10)>.5)
trait <- rnorm(10)+sex+as(b[,2],"vector")+as(b[,2],"vector")*sex*5
apply2dfo(trait~SNP*sex,dfodata=b,anFUN="lm",procFUN="process_lm_output")
}
\seealso{
  \link{apply2dfo}
}

