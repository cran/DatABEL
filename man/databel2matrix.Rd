\name{databel2matrix}
\alias{databel2matrix}
\title{converts 'databel' to matrix...}
\usage{databel2matrix(from, rows, cols)}
\description{converts 'databel' to matrix}
\details{Converts regular R matrix to \code{\linkS4class{databel}} object. 
This is the procedure used by "as" converting to DatABEL objects, 
in which case a temporary file name is created}
\value{object of \code{\linkS4class{matrix}} class}
\author{Stepan Yakovenko}
\arguments{\item{from}{'databel' matrix}
\item{rows}{which rows to include}
\item{cols}{which columns to include}}
