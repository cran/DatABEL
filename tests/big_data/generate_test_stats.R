#
# note that real test data now changed
# 1) *_varsum named *_obssum and vica versa
# 2) *_submatrix was transposed
#


library(DatABEL)

filename <- "ERF.merlin.22.collected.ped.out.mldose"
pt <- proc.time()
x <- databel_base_R(filename,64)
proc.time()-pt

pt <- proc.time()
obssum <- rep(0,dim(x)[1])
varsum <- rep(0,dim(x)[2])
for (i in 1:dim(x)[2]) {
  tmp <- as.vector(x[,i])
  obssum <- obssum + tmp
  varsum[i] <- sum(tmp)
}
proc.time()-pt
varsum[1:10]
obssum[1:10]
cat(varsum,file=paste(filename,"_varsum",sep=""),sep=" ")
cat(obssum,file=paste(filename,"_obssum",sep=""),sep=" ")
cat(get_dimnames(x)[[2]],file=paste(filename,"_varnames",sep=""),sep=" ")
cat(get_dimnames(x)[[1]],file=paste(filename,"_obsnames",sep=""),sep=" ")

selvar <- sort(sample(1:dim(x)[2],round(dim(x)[2]/1000)))
selobs <- sort(sample(1:dim(x)[1],round(dim(x)[1]/10)))
if (length(selvar) != length(unique(selvar))) stop("non-unique names in var")
if (length(selobs) != length(unique(selobs))) stop("non-unique names in obs")
testmtr <- x[selobs,selvar]
dim(testmtr)
dimnames(testmtr) <- list(get_dimnames(x)[[1]][selobs],get_dimnames(x)[[2]][selvar])
write.table(t(testmtr),file=paste(filename,"_submatrix",sep=""),row.names=T,col.names=T,quote=F)

