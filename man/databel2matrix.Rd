% Generated by roxygen2 (4.1.0): do not edit by hand
% Please edit documentation in R/databel2matrix.R
\name{databel2matrix}
\alias{databel2matrix}
\title{converts 'databel' to matrix}
\usage{
databel2matrix(from, rows, cols)
}
\arguments{
\item{from}{'databel' matrix}

\item{rows}{which rows to include}

\item{cols}{which columns to include}
}
\value{
object of \code{\linkS4class{matrix}} class
}
\description{
Converts a \code{\linkS4class{databel}} object to a regular R matrix.
This is the procedure used by the "as" converting to DatABEL objects,
in which case a temporary file name is created.
}
\author{
Stepan Yakovenko
}

