\name{extract_text_file_columns}
\alias{extract_text_file_columns}
\title{extract_text_file_columns}
\usage{extract_text_file_columns(file, whichcols)}
\description{extracts columns from text file}
\details{Extracts a column from text file to a matrix.
If in a particular file line the number of columns is less
then a column specified, returns last column!}
\value{matrix of strings with values from that columns}
\arguments{\item{file}{file name}
\item{whichcols}{which columns to extract}}
