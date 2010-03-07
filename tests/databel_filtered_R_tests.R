library(DatABEL)
source("test_funs.R")

NVAR <- 1000
NOBS <- 300

# generate test data
dta <- matrix(rnorm(NVAR*NOBS),ncol=NVAR,nrow=NOBS)
dim(dta)

# create databel_fileterd_R based on test data in different ways and test
x <- as(dta,"databel_base_R")
x <- databel_filtered_R(x)
disconnect(x)
connect(x)
x
class(x)
testobject(dta,as(x,"matrix"))

# make identical
dta <- as(x,"matrix")
testobject(dta,as(x,"matrix"))

# submatrix
rCol <- round(runif(floor(ncol(x)/10),min=1,max=ncol(x)))
rCol
rRow <- round(runif(floor(nrow(x)/10),min=1,max=nrow(x)))
rRow

dta1 <- dta[rRow,rCol]
x1 <- x[rRow,rCol]
testobject(dta1,as(x1,"matrix"))

dta1 <- dta[sort(rRow),sort(rCol)]
x1 <- x[sort(rRow),sort(rCol)]
testobject(dta1,as(x1,"matrix"))

rCol <- round(runif(floor(ncol(x1)/10),min=1,max=ncol(x1)))
rCol
rRow <- round(runif(floor(nrow(x1)/10),min=1,max=nrow(x1)))
rRow

dta2 <- dta1[rRow,rCol]
x2 <- x1[rRow,rCol]
testobject(dta2,as(x2,"matrix"))


logCol <- (runif(ncol(dta))<=0.1)
while (!any(logCol)) logCol <- (runif(ncol(dta))<=0.1)
logRow <- (runif(nrow(dta))<=0.1)
while (!any(logRow)) logRow <- (runif(nrow(dta))<=0.1)
table(logCol)
table(logRow)

dta1 <- dta[logRow,logCol]
x1 <- x[logRow,logCol]
testobject(dta1,as(x1,"matrix"))

logCol <- (runif(ncol(dta1))<=0.1)
while (!any(logCol)) logCol <- (runif(ncol(dta1))<=0.1)
logRow <- (runif(nrow(dta1))<=0.1)
while (!any(logRow)) logRow <- (runif(nrow(dta1))<=0.1)
table(logCol)
table(logRow)

dta2 <- dta1[logRow,logCol]
x2 <- x1[logRow,logCol]
testobject(dta2,as(x2,"matrix"))

dta1 <- dta[c(T,F,F,F,F),c(F,F,F,F,T)]
dim(x)
x1 <- x[c(T,F,F,F,F),c(F,F,F,F,T)]
testobject(dta1,as(x1,"matrix"))

# submatrix and permute
# on integer 
# on logical
# on names

apply(dta1,FUN=sum,MAR=1)
#apply(x1,FUN=sum,MAR=1)

dn <- dimnames(x)
ndn <- dn
ndn[[1]] <- sample(dn[[1]],10)
ndn[[2]] <- sample(dn[[2]],5)
ndn
dta1 <- dta[ndn[[1]],ndn[[2]]]
x1 <- x[ndn[[1]],ndn[[2]]]
dta1
x1
testobject(dta1,as(x1,"matrix"))

system("rm test*fv* tmp*fv*")


