\name{apply2dfo}
\alias{apply2dfo}
\title{applies a function to 'databel_filtered_R' object...}
\usage{apply2dfo(..., dfodata, anFUN="lm", MAR=2, procFUN, outclass="matrix",
    outfile, type="DOUBLE", transpose=TRUE)}
\description{applies a function to 'databel_filtered_R' object}
\details{An iterator applying a user-defined function to 
databel_filtered_R - class object}
\arguments{\item{dfodata}{'databel_filtered-R' object which is 
iterated over}
\item{anFUN}{user-defined analysis function}
\item{MAR}{which margin to iteracte over (default = 2, 
usually these are 'columns' used to store SNP data)}
\item{procFUN}{function to process the output and present 
that as a fixed-number-of-columns matrix or fixed-length 
vector. Can be missing if standard functions listed below are 
used. Pre-defined processors included are "process_lm_output"
(can process functions "lm", "glm", "coxph") and "process_simple_output"
(process output from "sum", "prod", "sum_not_NA" [no. non-missing obs], 
"sum_NA" [no. missing obs.])}
\item{outclass}{output to ("matrix" or "databel_filtered_R")}
\item{outfile}{if output class is "databel_filtered_R", the generated 
object is bond to the outfile}
\item{type}{if output class is "databel_filtered_R", what data 
tyoe to use for storage}
\item{transpose}{whether to transpose the output}
\item{...}{arguments passed to the anFUN}}
\examples{a <- matrix(rnorm(50),10,5)
rownames(a) <- paste("id",1:10,sep="")
colnames(a) <- paste("snp",1:5,sep="")
b <- as(a,"databel_filtered_R")
apply(a,FUN="sum",MAR=2)
apply2dfo(SNP,dfodata=b,anFUN="sum")
tA <- apply2dfo(SNP,dfodata=b,anFUN="sum",outclass="databel_filtered_R",outfile="tmpA")
tA
as(tA,"matrix")
apply2dfo(SNP,dfodata=b,anFUN="sum",transpose=FALSE)
tB <- apply2dfo(SNP,dfodata=b,anFUN="sum",transpose=FALSE,outclass="databel_filtered_R",outfile="tmpB")
tB
as(tB,"matrix")

sex <- 1*(runif(10)>.5)
trait <- rnorm(10)+sex+as(b[,2],"vector")+as(b[,2],"vector")*sex*5
apply2dfo(trait~SNP*sex,dfodata=b,anFUN="lm")
tC <- apply2dfo(trait~SNP*sex,dfodata=b,anFUN="lm",outclass="databel_filtered_R",outfile="tmpC")
tC
as(tC,"matrix")
apply2dfo(trait~SNP*sex,dfodata=b,anFUN="lm",transpose=FALSE)
tD <- apply2dfo(trait~SNP*sex,dfodata=b,anFUN="lm",transpose=FALSE,outclass="databel_filtered_R",outfile="tmpD")
tD
as(tD,"matrix")}
