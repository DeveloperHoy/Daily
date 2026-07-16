package com.example;

import java.io.IOException;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/**
 * Servlet에서 JSP로 데이터를 넘겨 화면을 그리는 예제 (forward 방식 -> URL 안 바뀜)
 */
@WebServlet("/greet")
public class GreetServlet extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
        String name = req.getParameter("name");
        req.setAttribute("message", "환영합니다, " + name + "님");
        req.getRequestDispatcher("/greeting.jsp").forward(req, resp);
    }
}
