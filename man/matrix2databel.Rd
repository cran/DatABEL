\name{matrix2databel}
\alias{matrix2databel}
\title{converts matrix to 'databel'...}
\usage{matrix2databel(from, filename, cachesizeMb=64, type="DOUBLE",
    readonly=FALSE)}
\description{converts matrix to 'databel'}
\details{Converts regular R matrix to \code{\linkS4class{databel}} object. 
This is the procedure used by "as" converting to DatABEL objects, 
in which case a temporary file name is created}
\value{object of class \code{\linkS4class{databel}}}
\author{Yurii Aulchenko}
\arguments{\item{from}{R matrix}
\item{filename}{which FILEVECTOR BASE file name to use}
\item{cachesizeMb}{cache size to be used when accessing the object}
\item{type}{type of data to use for storage ("DOUBLE", "FLOAT", "INT", 
"UNSIGNED_INT", "UNSIGNED_SHORT_INT", "SHORT_INT")}
\item{readonly}{whether to generate new 'databel' in read only mode}}
