\name{process_lm_output}
\title{'apply2dfo'-associated functions...}
\usage{process_lm_output(lmo,verbosity=2) 
process_simple_output(o) 
sum_not_NA(x) 
sum_NA(x)}
\description{'apply2dfo'-associated functions}
\details{A number of functions used in 
conjunction with 'apply2dfo'. 
Standardly supported apply2dfo's anFUN analysis functions
include 'lm', 'glm', 'coxph', 'sum', 'prod', "sum_not_NA" 
(no. non-missing obs), and "sum_NA" (no. missing obs.). 
Pre-defined processing functions include "process_lm_output"
(can process functions "lm", "glm", "coxph") and "process_simple_output"
(process output from "sum", "prod", "sum_not_NA", 
"sum_NA")}
\alias{process_lm_output}
\alias{process_simple_output}
\alias{sum_not_NA}
\alias{sum_NA}
\seealso{\link{apply2dfo}}
\arguments{\item{lmo}{object returned by analysis with "lm", "glm", etc.}
\item{verbosity}{verbosity}
\item{o}{output for processing}
\item{x}{vector of data on which function is applied}}
\examples{a <- matrix(rnorm(50),10,5)
rownames(a) <- paste("id",1:10,sep="")
colnames(a) <- paste("snp",1:5,sep="")
b <- as(a,"databel_filtered_R")
apply(a,FUN="sum",MAR=2)
apply2dfo(SNP,dfodata=b,anFUN="sum",procFUN="process_simple_output")
apply2dfo(SNP,dfodata=b,anFUN="sum",transpose=FALSE)

sex <- 1*(runif(10)>.5)
trait <- rnorm(10)+sex+as(b[,2],"vector")+as(b[,2],"vector")*sex*5
apply2dfo(trait~SNP*sex,dfodata=b,anFUN="lm",procFUN="process_lm_output")}
