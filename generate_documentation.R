library(roxygen)
setwd("R")
system("rm -r DatABEL")
files <- system("ls *R",int=T)
files
class_files <- files[grep("*class.R",files)]
class_files <- c(class_files,files[grep("*internal.R",files)])
class_filec <- c(class_files,files[grep("zzz.R",files)])
class_files
good_files <- files[!(files %in% class_files)]
good_files
package.skeleton("DatABEL",code_files=good_files)
roxygenize("DatABEL",roxygen.dir="DatABEL",copy=F,unlink=F)
#package.skeleton("DatABEL",code_files=class_files,force=TRUE)
system("cp DatABEL/man/*Rd ../man/.")
system("rm -r DatABEL")
