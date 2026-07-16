package com.example;

import java.io.IOException;
import java.io.PrintWriter;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/**
 * GET: http://localhost:8080/myapp/hello?name=에이치
 * POST: 폼에서 name 전송하면 처리 후 리다이렉트
 */
@WebServlet("/hello")
public class HelloServlet extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
        resp.setContentType("text/html; charset=UTF-8");
        PrintWriter out = resp.getWriter();
        String name = req.getParameter("name");
        out.println("<h1>안녕, " + (name != null ? name : "손님") + "!</h1>");
        out.println("<a href='greet?name=" + (name != null ? name : "손님") + "'>JSP로 이동</a>");
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
        String name = req.getParameter("name");
        resp.sendRedirect("hello?name=" + name);
    }
}
